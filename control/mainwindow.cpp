#include "mainwindow.h"
#include "settingsdialog.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include <omkit/utils.h>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    setWindowTitle("Offline-наставник. Система контроля v." + getVersion());

    settingsDialog = new SettingsDialog(this);
    settingsDialog->hide();

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
}

void MainWindow::on_settingsAction_triggered()
{
    settingsDialog->initUI();
    settingsDialog->exec();
}
