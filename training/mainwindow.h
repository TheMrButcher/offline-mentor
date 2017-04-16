#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <omkit/section.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginForm;
class SectionsForm;
class Solution;
class TrainingForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void onLogin();
    void openSection(const Section& section);
    void onSolutionSaved(const Solution& solution);

    void on_tabWidget_tabCloseRequested(int index);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    void select(QWidget* widget);
    bool closePage(TrainingForm* trainingForm);

    Ui::MainWindow *ui;
    LoginForm* loginForm;
    SectionsForm* sectionsForm;
    QHash<QUuid, QWidget*> openedPages;
};

#endif // MAINWINDOW_H
