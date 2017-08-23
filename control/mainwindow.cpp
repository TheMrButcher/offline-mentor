#include "mainwindow.h"
#include "settingsdialog.h"
#include "solutionsform.h"
#include "groupsform.h"
#include "solutionexplorer.h"
#include "trainingcreationwizard.h"
#include "section_utils.h"
#include "solution_utils.h"
#include "group_utils.h"
#include "settings.h"
#include "settingswizard.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
#include <omkit/omkit.h>
#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/ui_utils.h>
#include <omkit/solution.h>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    setWindowTitle("Offline-наставник. Система контроля v." + getVersion());

    OMKit::instance().init();

    settingsDialog = new SettingsDialog(this);
    settingsDialog->hide();
    connect(settingsDialog, SIGNAL(requestedSettingsWizard()),
            this, SLOT(execSettingsWizard()));

    solutionsForm = new SolutionsForm(this);
    int tabIndex = ui->tabWidget->addTab(solutionsForm, "Сводная таблица");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);
    connect(solutionsForm, SIGNAL(requestedOpen(Solution)),
            this, SLOT(openSolution(Solution)));

    groupsForm = new GroupsForm(this);
    tabIndex = ui->tabWidget->addTab(groupsForm, "Группы");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);
    connect(groupsForm, SIGNAL(groupCollectionChanged()),
            solutionsForm, SLOT(onGroupCollectionChanged()));
    connect(groupsForm, SIGNAL(groupAdded(QUuid)),
            solutionsForm, SLOT(onGroupAdded(QUuid)));
    connect(settingsDialog, SIGNAL(groupsPathChanged()),
            groupsForm, SLOT(onGroupsPathChanged()));

    trainingCreationWizard = new TrainingCreationWizard(this);
    trainingCreationWizard->hide();

    connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(groupsForm, SIGNAL(groupAdded(QUuid)),
            trainingCreationWizard, SLOT(onGroupAdded(QUuid)));
    connect(trainingCreationWizard, SIGNAL(groupCreationRequested()),
            groupsForm, SLOT(createGroup()));

    QTimer::singleShot(0, this, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    auto& settings = Settings::instance();
    if (!settings.read())
        QMessageBox::warning(this, "Отсутствует файл с настройками",
                             "Не удалось прочесть файл с настройками. "
                             "Возможно, приложение установлено не полностью. "
                             "Приложение может работать некорректно.");

    if (settings.isFirstUsage)
        execSettingsWizard();

    loadGroups();
    groupsForm->load();
    solutionsForm->reload();
}

void MainWindow::openSolution(const Solution& solution)
{
    SolutionExplorer* solutionExplorer = new SolutionExplorer(this);
    solutionExplorer->setSolution(solution);
    connect(solutionExplorer, SIGNAL(authorRenamed()),
            solutionsForm, SLOT(reload()));
    QString tabTitle = QString("\"%1\" / %2")
            .arg(trim(getSections()[solution.sectionId].name, 15))
            .arg(trim(solution.userName, 10));
    int tabIndex = ui->tabWidget->addTab(solutionExplorer, tabTitle);
    ui->tabWidget->setCurrentIndex(tabIndex);
}

void MainWindow::execSettingsWizard()
{
    if (!settingsWizard)
        settingsWizard = new SettingsWizard(this);
    settingsWizard->restart();
    settingsWizard->exec();
    settingsDialog->initUI();
}

void MainWindow::on_settingsAction_triggered()
{
    settingsDialog->initUI();
    settingsDialog->exec();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    if (widget != solutionsForm && widget != groupsForm)
        delete widget;
}

void MainWindow::on_trainingCreationWizardAction_triggered()
{
    trainingCreationWizard->restart();
    trainingCreationWizard->exec();
}

void MainWindow::on_aboutAction_triggered()
{
    if (!aboutDialog)
        aboutDialog = new AboutDialog(this);
    aboutDialog->exec();
}

void MainWindow::on_importSolutionArchiveAction_triggered()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Путь к архиву", Settings::instance().lastPath, "Архив (*.zip)");
    if (!path.isEmpty()) {
        Settings::instance().updateLastPath(QFileInfo(path).absolutePath());
        if (!importSolutionsFromArchive(path)) {
            QMessageBox::warning(this, "Ошибка при загрузке",
                                 "Не удалось загрузить ответы из указанного архива.");
            return;
        }
        solutionsForm->reload();
    }
}

void MainWindow::on_importSectionFolderAction_triggered()
{
    QString path = QFileDialog::getExistingDirectory(
                this, "Путь к папке", Settings::instance().lastPath);
    if (!path.isEmpty()) {
        Settings::instance().updateLastPath(path);
        auto importedSectionNames = importSectionsFromFolder(path);
        if (importedSectionNames.isEmpty()) {
            QMessageBox::warning(this, "Ошибка при загрузке",
                                 "Не удалось загрузить разделы из указанной папки.");
            return;
        } else {
            QMessageBox::information(this, "Загрузка разделов",
                                     QString("Было загружено %1 разделов: \n%2")
                                     .arg(importedSectionNames.size())
                                     .arg(importedSectionNames.join('\n')));
        }
        solutionsForm->reload();
    }
}

void MainWindow::on_importSectionArchiveAction_triggered()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Путь к архиву", Settings::instance().lastPath, "Архив (*.zip)");
    if (!path.isEmpty()) {
        Settings::instance().updateLastPath(QFileInfo(path).absolutePath());
        auto importedSectionNames = importSectionsFromArchive(path);
        if (importedSectionNames.isEmpty()) {
            QMessageBox::warning(this, "Ошибка при загрузке",
                                 "Не удалось загрузить разделы из указанного архива.");
            return;
        } else {
            QMessageBox::information(this, "Загрузка разделов",
                                     QString("Было загружено %1 разделов: \n%2")
                                     .arg(importedSectionNames.size())
                                     .arg(importedSectionNames.join('\n')));
        }
        solutionsForm->reload();
    }
}
