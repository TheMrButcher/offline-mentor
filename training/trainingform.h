#ifndef TRAININGFORM_H
#define TRAININGFORM_H

#include <omkit/section.h>
#include <omkit/solution.h>

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
    void onAnswerEntered(QListWidgetItem* caseItem);
    void toMentorAnswer(QListWidgetItem* caseItem);
    void backToQuestion(QListWidgetItem* caseItem);
    void next(QListWidgetItem* caseItem);

private:
    void openQuestionPage(int pageId);

    Ui::TrainingForm *ui;

    struct NodeDescriptor {
        int questionPageId;
        int mentorAnswerPageId;
        QListWidgetItem* nextItem;
    };

    Section section;
    Solution localSolution;
    QHash<QListWidgetItem*, NodeDescriptor> nodes;
};

#endif // TRAININGFORM_H
