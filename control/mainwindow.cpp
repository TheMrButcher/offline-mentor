#include "mainwindow.h"
#include "settingsdialog.h"
#include "solutionsform.h"
#include "solutionexplorer.h"
#include "section_utils.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include <omkit/utils.h>
#include <omkit/solution.h>
#include <QMessageBox>
#include <QTimer>
#include <QStringList>

namespace {
QString trim(QString str, int len)
{
    if (str.size() < len)
        return str;
    QStringList parts = str.split(' ', QString::SkipEmptyParts);
    if (parts.isEmpty())
        return QString();
    str = parts.front();
    int index = 1;
    while (index < parts.size() && str.size() + parts[index].size() + 1 <= len)
        str += " " + parts[index++];
    if (str.size() > len)
        str = str.left(len);
    return str.left(len) + "...";
}
}

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
    connect(solutionsForm, SIGNAL(requestedOpen(Solution)),
            this, SLOT(openSolution(Solution)));

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
