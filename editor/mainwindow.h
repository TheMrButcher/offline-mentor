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
class RichTextEdit;
class QFontComboBox;
class QComboBox;

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
    void setBold(bool bold);
    void setItalic(bool italic);
    void setUnderline(bool underline);
    void setFontFamily(QString fontFamily);
    void setFontSize(QString fontSizeStr);
    void showAbout();
    void loadSettings();
    void openSection(const Section& section);
    void onSectionSaved(const Section& section);
    void onTextEditInFocus(bool inFocus);
    void onFontChanged(const QFont& font);

    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);

private:
    bool isSectionsFormCurrent() const;
    RichTextEdit* currentTextEdit();
    void setTextEditButtonsEnabled(bool enabled);
    void updateTextEditButtons();
    void updateFontButtons(const QFont& font);

    Ui::MainWindow *ui;
    QFontComboBox* fontComboBox;
    QComboBox* fontSizeComboBox;

    SectionsForm* sectionsForm;
    CreateSectionDialog* createSectionDialog = nullptr;
    AboutDialog* aboutDialog = nullptr;
    QHash<QUuid, QWidget*> openedPages;
};

#endif // MAINWINDOW_H
