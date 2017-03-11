#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "loginform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Offline-наставник. Тренажер v." + getVersion());

    loginForm = new LoginForm(this);
    ui->stackedWidget->addWidget(loginForm);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
