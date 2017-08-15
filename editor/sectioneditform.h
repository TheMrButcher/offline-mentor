#ifndef SECTIONEDITFORM_H
#define SECTIONEDITFORM_H

#include <omkit/section.h>

#include <QWidget>
#include <QSet>

namespace Ui {
class SectionEditForm;
}

class QTreeWidgetItem;
class CasePage;
class TextEditorPage;
class QTextCharFormat;
class RichTextEdit;

class SectionEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionEditForm(QWidget *parent = 0);
    ~SectionEditForm();

    Section section() const;
    void setSection(const Section& section);
    QString sectionName() const;
    QUuid sectionId() const;
    QDir sectionDir() const;
    bool isTextEditInFocus() const;
    bool isCaseInFocus() const;
    RichTextEdit* currentTextEdit() const;
    bool hasChanges() const;

    bool isImageHolderInFocus() const;
    CaseImage currentImage() const;
    void setCurrentImage(const CaseImage& image);

signals:
    void sectionSaved(const Section& section);
    void textEditInFocus(bool);
    void caseInFocus(bool);
    void fontChanged(const QFont& font);
    void selectionChanged();
    void cursorPositionChanged();
    void undoAvailable(bool available);
    void redoAvailable(bool available);
    void modificationChanged(bool changed);
    void requestedCurrentImageMenu();

public slots:
    void save();
    void addCase();
    void removeCurrentCase();

private slots:
    void onCharFormatChanged(const QTextCharFormat& format);
    void onSelectionChanged();
    void onCursorPositionChanged();
    void onUndoAvailable(bool available);
    void onRedoAvailable(bool available);
    void onModificationChanged(bool changed);
    void onNameChanged();
    void openCurrentQuestionPage();
    void openCurrentAnswerPage();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem* previous);
    void on_nameEdit_textEdited(const QString &arg1);

private:
    void setSectionName(QString name);
    QTreeWidgetItem* addCase(const Case& caseValue);
    void generateFileNames(Case& c);
    Section sectionFromUI() const;
    void select(QWidget* widget);
    void connectPage(TextEditorPage* page);
    void removeImage(const CaseImage& caseImage);

    Ui::SectionEditForm *ui;

    Section originalSection;
    QTreeWidgetItem* rootItem;
    QTreeWidgetItem* totalItem;
    TextEditorPage* totalEditorPage;

    TextEditorPage* currentTextEditorPage = nullptr;

    struct CaseItems {
        QTreeWidgetItem* root;
        QTreeWidgetItem* question;
        QTreeWidgetItem* answer;
    };

    struct CasePages {
        CasePage* mainPage;
        TextEditorPage* questionPage;
        TextEditorPage* answerPage;
    };

    struct NodeDescriptor {
        QWidget* page;
        TextEditorPage* textEditorPage;
        CaseItems items;
        CasePages pages;
    };

    QHash<QTreeWidgetItem*, NodeDescriptor> nodes;
    QHash<TextEditorPage*, CaseImage> images;
    QSet<QObject*> modifiedDocuments;
    bool modifiedNames = false;
    bool modifiedImages = false;
};

#endif // SECTIONEDITFORM_H
