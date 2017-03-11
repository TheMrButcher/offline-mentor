#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Offline-наставник. Тренажер v." + getVersion());
}

MainWindow::~MainWindow()
{
    delete ui;
}
