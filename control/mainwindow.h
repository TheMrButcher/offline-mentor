#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void on_settingsAction_triggered();

private:
    Ui::MainWindow *ui;

    SettingsDialog* settingsDialog;
};

#endif // MAINWINDOW_H
