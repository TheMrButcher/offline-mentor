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
    void cut();
    void copy();
    void paste();
    void align(bool toggled);
    void list(bool toggled);
    void undo();
    void redo();
    void showAbout();
    void loadSettings();
    void openSection(const Section& section);
    void onSectionSaved(const Section& section);
    void onTextEditInFocus(bool inFocus);
    void onFontChanged(const QFont& font);
    void onSelectionChanged();
    void updatePasteButton();
    void updateHistoryButtons();
    void onCursorPositionChanged();
    void onHistoryAvailable(bool);
    void onModificationChanged(bool changed);

    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    bool isSectionsFormCurrent() const;
    SectionEditForm* currentSectionEditForm();
    RichTextEdit* currentTextEdit();
    void setTextEditButtonsEnabled(bool enabled);
    void updateFontButtons(const QFont& font);
    void setCopyAndCutButtonsEnabled(bool enabled);
    void updateAlignmentButtons(Qt::Alignment alignment);
    void updateListButtons();
    bool closePage(SectionEditForm* sectionEditForm);

    Ui::MainWindow *ui;
    QFontComboBox* fontComboBox;
    QComboBox* fontSizeComboBox;

    SectionsForm* sectionsForm;
    CreateSectionDialog* createSectionDialog = nullptr;
    AboutDialog* aboutDialog = nullptr;
    QHash<QUuid, QWidget*> openedPages;
};

#endif // MAINWINDOW_H
