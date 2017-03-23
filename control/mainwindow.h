#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Solution;
class SettingsDialog;
class SolutionsForm;
class TrainingCreationWizard;

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

    void on_trainingCreationWizardAction_triggered();

private:
    Ui::MainWindow *ui;

    SettingsDialog* settingsDialog;
    SolutionsForm* solutionsForm;
    TrainingCreationWizard* trainingCreationWizard;
};

#endif // MAINWINDOW_H
