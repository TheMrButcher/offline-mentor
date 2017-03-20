#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <omkit/utils.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    setWindowTitle("Offline-наставник. Система контроля v." + getVersion());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
