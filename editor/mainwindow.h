#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "section.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SectionsForm;
class SectionEditForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadSettings();
    void openSection(const Section& section);

    void on_saveAction_triggered();

private:
    void select(QWidget* widget);

    Ui::MainWindow *ui;

    SectionsForm* sectionsForm;
    SectionEditForm* sectionEditForm;
};

#endif // MAINWINDOW_H
