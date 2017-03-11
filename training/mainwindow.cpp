#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "loginform.h"
#include "sectionsform.h"
#include "settings.h"

#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Offline-наставник. Тренажер v." + getVersion());

    loginForm = new LoginForm(this);
    ui->stackedWidget->addWidget(loginForm);

    sectionsForm = new SectionsForm(this);
    ui->stackedWidget->addWidget(sectionsForm);

    select(loginForm);

    connect(loginForm, SIGNAL(login()), this, SLOT(onLogin()));

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
    sectionsForm->setSections(Section::loadSections());
}

void MainWindow::onLogin()
{
    sectionsForm->setUserName(loginForm->userName());
    select(sectionsForm);
}

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
