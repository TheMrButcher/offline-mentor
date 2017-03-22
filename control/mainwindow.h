#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Solution;
class SettingsDialog;
class SolutionsForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void openSolution(const Solution& solution);
    void on_settingsAction_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;

    SettingsDialog* settingsDialog;
    SolutionsForm* solutionsForm;
};

#endif // MAINWINDOW_H
