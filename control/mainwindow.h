#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Solution;
class SettingsDialog;
class SolutionsForm;
class GroupsForm;
class TrainingCreationWizard;
class SettingsWizard;
class AboutDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void openSolution(const Solution& solution);
    void execSettingsWizard();
    void on_settingsAction_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_trainingCreationWizardAction_triggered();
    void on_aboutAction_triggered();

    void on_importSolutionArchiveAction_triggered();

    void on_importSectionFolderAction_triggered();

    void on_importSectionArchiveAction_triggered();

private:
    Ui::MainWindow *ui;

    SettingsDialog* settingsDialog;
    SolutionsForm* solutionsForm;
    GroupsForm* groupsForm;
    TrainingCreationWizard* trainingCreationWizard;
    SettingsWizard* settingsWizard = nullptr;
    AboutDialog* aboutDialog = nullptr;
};

#endif // MAINWINDOW_H
