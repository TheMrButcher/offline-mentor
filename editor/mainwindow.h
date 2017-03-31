#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <omkit/section.h>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class SectionsForm;
class CreateSectionDialog;
class AboutDialog;
class SectionEditForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void create();
    void open();
    void save();
    void selectAll();
    void clearFormat();
    void showAbout();
    void loadSettings();
    void openSection(const Section& section);
    void onSectionSaved(const Section& section);
    void onTextEditInFocus(bool inFocus);

    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);

private:
    bool isSectionsFormCurrent() const;
    SectionEditForm* currentForm();
    void setTextEditButtonsEnabled(bool enabled);

    Ui::MainWindow *ui;

    SectionsForm* sectionsForm;
    CreateSectionDialog* createSectionDialog = nullptr;
    AboutDialog* aboutDialog = nullptr;
    QHash<QUuid, QWidget*> openedPages;
};

#endif // MAINWINDOW_H
