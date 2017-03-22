#ifndef ANSWERPAGE_H
#define ANSWERPAGE_H

#include <QWidget>

namespace Ui {
class AnswerPage;
}

class Section;
class Solution;

enum class AnswerStatus {
    OK,
    Absent,
    Error
};

class AnswerPage : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerPage(QWidget *parent = 0);
    ~AnswerPage();

    AnswerStatus load(
            const Section& section, const Solution& solution, int caseIndex);

signals:
    void requestedCase(int caseIndex);

private slots:
    void on_prevButton_clicked();

    void on_nextButton_clicked();

private:
    Ui::AnswerPage *ui;
    int caseIndex;
};

#endif // ANSWERPAGE_H
