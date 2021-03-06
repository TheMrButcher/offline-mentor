#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include "sectionsform.h"
#include "trainingform.h"
#include "user_utils.h"
#include "group_utils.h"
#include "solution_utils.h"
#include "settings.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/omkit.h>

#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Offline наставник. Тренажер v." + getVersion());

    OMKit::instance().init();
    qDebug() << "OMKit: v." << OMKit::instance().getVersion();

    loginForm = new LoginForm(this);
    ui->stackedWidget->addWidget(loginForm);

    sectionsForm = new SectionsForm(this);
    int tabIndex = ui->tabWidget->addTab(sectionsForm, "Список разделов");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);

    ui->stackedWidget->setCurrentWidget(loginForm);

    connect(loginForm, SIGNAL(login()), this, SLOT(onLogin()));
    connect(sectionsForm, SIGNAL(requestedOpen(Section)), this, SLOT(openSection(Section)));

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

    loadGroups();
    loginForm->init();
    sectionsForm->setSections(Section::findAll(settings.sectionsPath));
}

void MainWindow::onLogin()
{
    showMaximized();
    setUserName(loginForm->userName());
    loadSolutions();
    sectionsForm->setUserName(loginForm->userName());
    sectionsForm->updateProgress();
    ui->tabWidget->setCurrentWidget(sectionsForm);
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::openSection(const Section& section)
{
    if (openedPages.contains(section.id)) {
        ui->tabWidget->setCurrentWidget(openedPages[section.id]);
        return;
    }
    TrainingForm* trainingForm = new TrainingForm(this);
    if (!trainingForm->setSection(section)) {
        QMessageBox::warning(this, "Ошибка при открытии",
                             "Не удалось открыть раздел.");
        delete trainingForm;
        return;
    }
    ui->tabWidget->addTab(trainingForm, trim(section.name, 16));
    ui->tabWidget->setCurrentWidget(trainingForm);
    openedPages[section.id] = trainingForm;
    connect(trainingForm, SIGNAL(savedSolution(Solution)),
            this, SLOT(onSolutionSaved(Solution)));
}

void MainWindow::onSolutionSaved(const Solution&)
{
    sectionsForm->updateProgress();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index == ui->tabWidget->indexOf(sectionsForm))
        return;
    auto widget = ui->tabWidget->widget(index);
    closePage((TrainingForm*)widget);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    int sectionsFormIndex = ui->tabWidget->indexOf(sectionsForm);
    for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
        if (i == sectionsFormIndex)
            continue;
        auto trainingForm = (TrainingForm*)ui->tabWidget->widget(i);
        if (!closePage(trainingForm)) {
            event->ignore();
            return;
        }
    }
    event->accept();
}

bool MainWindow::closePage(TrainingForm* trainingForm)
{
    if (!trainingForm->tryClose())
        return false;
    auto id = trainingForm->sectionId();
    openedPages.remove(id);
    delete trainingForm;
    return true;
}
