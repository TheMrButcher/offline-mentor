#ifndef SECTIONEDITFORM_H
#define SECTIONEDITFORM_H

#include "section.h"

#include <QWidget>
#include <memory>

namespace Ui {
class SectionEditForm;
}

class QTreeWidgetItem;
class CasePage;
class TextEditorPage;

class SectionEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionEditForm(QWidget *parent = 0);
    ~SectionEditForm();

    Section section() const;
    void setSection(const Section& section);

public slots:
    void save();

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

    Ui::SectionEditForm *ui;

    Section originalSection;
    QTreeWidgetItem* rootItem;

    struct CasePages {
        CasePage* mainPage;
        TextEditorPage* questionPage;
        TextEditorPage* answerPage;
    };

    struct NodeDescriptor {
        int pageId;
        CasePages pages;
    };

    QHash<QTreeWidgetItem*, NodeDescriptor> nodes;
};

#endif // SECTIONEDITFORM_H
