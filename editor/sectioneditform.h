#ifndef SECTIONEDITFORM_H
#define SECTIONEDITFORM_H

#include <omkit/section.h>

#include <QWidget>
#include <memory>

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
    QUuid sectionId() const;
    bool isTextEditInFocus() const;
    RichTextEdit* currentTextEdit() const;

signals:
    void sectionSaved(const Section& section);
    void textEditInFocus(bool);
    void fontChanged(const QFont& font);

public slots:
    void save();
    void onCharFormatChanged(const QTextCharFormat& format);

private slots:
    void on_addCaseButton_clicked();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem* previous);
    void on_nameEdit_textEdited(const QString &arg1);

private:
    void setSectionName(QString name);
    void addCase(const Case& caseValue);
    void generateFileNames(Case& c);
    Section sectionFromUI() const;
    void select(QWidget* widget);
    void connectPage(TextEditorPage* page);

    Ui::SectionEditForm *ui;

    Section originalSection;
    QTreeWidgetItem* rootItem;
    QTreeWidgetItem* totalItem;
    TextEditorPage* totalEditorPage;

    TextEditorPage* currentTextEditorPage = nullptr;

    struct CasePages {
        CasePage* mainPage;
        TextEditorPage* questionPage;
        TextEditorPage* answerPage;
    };

    struct NodeDescriptor {
        int pageId;
        TextEditorPage* textEditorPage;
        CasePages pages;
    };

    QHash<QTreeWidgetItem*, NodeDescriptor> nodes;
};

#endif // SECTIONEDITFORM_H
