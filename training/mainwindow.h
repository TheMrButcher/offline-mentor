#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <omkit/section.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginForm;
class SectionsForm;
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

private:
    void select(QWidget* widget);

    Ui::MainWindow *ui;
    LoginForm* loginForm;
    SectionsForm* sectionsForm;
    TrainingForm* trainingForm;
};

#endif // MAINWINDOW_H
