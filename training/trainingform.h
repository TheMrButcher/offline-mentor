#ifndef TRAININGFORM_H
#define TRAININGFORM_H

#include <omkit/section.h>

#include <QWidget>
#include <QHash>

namespace Ui {
class TrainingForm;
}

class QListWidgetItem;

class TrainingForm : public QWidget
{
    Q_OBJECT

public:
    explicit TrainingForm(QWidget *parent = 0);
    ~TrainingForm();

    void setSection(const Section& section);

private slots:
    void on_listWidget_itemSelectionChanged();

private:
    Ui::TrainingForm *ui;

    struct NodeDescriptor {
        int questionPageId;
    };

    Section section;
    QHash<QListWidgetItem*, NodeDescriptor> nodes;
};

#endif // TRAININGFORM_H
