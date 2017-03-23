#include "mainwindow.h"
#include "settingsdialog.h"
#include "solutionsform.h"
#include "solutionexplorer.h"
#include "trainingcreationwizard.h"
#include "section_utils.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include <omkit/omkit.h>
#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/solution.h>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>

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

    solutionsForm = new SolutionsForm(this);
    int tabIndex = ui->tabWidget->addTab(solutionsForm, "Сводная таблица");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);
    connect(solutionsForm, SIGNAL(requestedOpen(Solution)),
            this, SLOT(openSolution(Solution)));

    trainingCreationWizard = new TrainingCreationWizard(this);
    trainingCreationWizard->hide();

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
    solutionsForm->reload();
}

void MainWindow::openSolution(const Solution& solution)
{
    SolutionExplorer* solutionExplorer = new SolutionExplorer(this);
    solutionExplorer->setSolution(solution);
    QString tabTitle = QString("\"%1\" / %2")
            .arg(trim(getSections()[solution.sectionId].name, 15))
            .arg(trim(solution.userName, 10));
    int tabIndex = ui->tabWidget->addTab(solutionExplorer, tabTitle);
    ui->tabWidget->setCurrentIndex(tabIndex);
}

void MainWindow::on_settingsAction_triggered()
{
    settingsDialog->initUI();
    settingsDialog->exec();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    if (widget != solutionsForm)
        delete widget;
}

void MainWindow::on_trainingCreationWizardAction_triggered()
{
    trainingCreationWizard->restart();
    trainingCreationWizard->exec();
}
