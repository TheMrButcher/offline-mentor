#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginForm;
class SectionsForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void onLogin();

private:\
    void select(QWidget* widget);

    Ui::MainWindow *ui;
    LoginForm* loginForm;
    SectionsForm* sectionsForm;
};

#endif // MAINWINDOW_H
