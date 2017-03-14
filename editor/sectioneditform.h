#ifndef SECTIONEDITFORM_H
#define SECTIONEDITFORM_H

#include "section.h"

#include <QWidget>
#include <memory>

namespace Ui {
class SectionEditForm;
}

class QTreeWidgetItem;

class SectionEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionEditForm(QWidget *parent = 0);
    ~SectionEditForm();

    Section section() const;
    void setSection(const Section& section);

private slots:
    void on_addCaseButton_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem* previous);

private:
    void addCase(const Case& caseValue);
    Section sectionFromUI() const;
    void select(QWidget* widget);

    Ui::SectionEditForm *ui;

    Section originalSection;
    QTreeWidgetItem* rootItem;

    enum class NodeType {
        CaseRoot
    };

    struct CaseData {
        Case value;
        QList<QMetaObject::Connection> connections;
        QList<QTreeWidgetItem*> items;
        QTreeWidgetItem* rootItem;

        void disconnect();
    };

    struct NodeDescriptor {
        NodeType type;
        QSharedPointer<CaseData> data;

        void set(NodeType type, QSharedPointer<CaseData> data)
        {
            this->type = type;
            this->data = data;
        }
    };

    QHash<QTreeWidgetItem*, NodeDescriptor> nodes;
};

#endif // SECTIONEDITFORM_H
