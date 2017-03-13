#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SectionsForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void select(QWidget* widget);

    Ui::MainWindow *ui;

    SectionsForm* sectionsForm;
};

#endif // MAINWINDOW_H
