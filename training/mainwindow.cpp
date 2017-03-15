#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include "sectionsform.h"
#include "trainingform.h"
#include "settings.h"

#include <omkit/utils.h>
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

    qDebug() << "OMKit: v." << OMKit::instance().getVersion();

    loginForm = new LoginForm(this);
    ui->stackedWidget->addWidget(loginForm);

    sectionsForm = new SectionsForm(this);
    ui->stackedWidget->addWidget(sectionsForm);

    trainingForm = new TrainingForm(this);
    ui->stackedWidget->addWidget(trainingForm);

    select(loginForm);

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
    sectionsForm->setUserName(loginForm->userName());
    select(sectionsForm);
}

void MainWindow::openSection(const Section& section)
{
    trainingForm->setSection(section);
    select(trainingForm);
}

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
