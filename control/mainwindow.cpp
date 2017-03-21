#include "mainwindow.h"
#include "settingsdialog.h"
#include "solutionsform.h"
#include "settings.h"
#include "section_utils.h"
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

    solutionsForm = new SolutionsForm(this);
    ui->tabWidget->addTab(solutionsForm, "Сводная таблица");

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
    loadSections();
    solutionsForm->reload();
}

void MainWindow::on_settingsAction_triggered()
{
    settingsDialog->initUI();
    settingsDialog->exec();
}
