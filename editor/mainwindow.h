#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <omkit/section.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SectionsForm;
class CreateSectionDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startCreation();
    void open();
    void save();
    void loadSettings();
    void onSectionCreated();
    void openSection(const Section& section);
    void onSectionSaved(const Section& section);

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_currentChanged(int index);

private:
    bool isSectionsFormCurrent() const;

    Ui::MainWindow *ui;

    SectionsForm* sectionsForm;
    CreateSectionDialog* createSectionDialog;
    QHash<QUuid, QWidget*> openedPages;
};

#endif // MAINWINDOW_H
