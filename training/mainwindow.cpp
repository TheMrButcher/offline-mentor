#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include "sectionsform.h"
#include "trainingform.h"
#include "user_utils.h"
#include "solution_utils.h"
#include "settings.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/omkit.h>

#include <QMessageBox>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    setWindowTitle("Offline-наставник. Тренажер v." + getVersion());

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
    loginForm->setUserName(settings.lastLogin);
    sectionsForm->setSections(Section::findAll(settings.sectionsPath));
}

void MainWindow::onLogin()
{
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
    trainingForm->setSection(section);
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
    auto id = ((TrainingForm*)widget)->sectionId();
    openedPages.remove(id);
    delete widget;
}
