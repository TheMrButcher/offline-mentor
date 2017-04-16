#ifndef QUESTIONPAGE_H
#define QUESTIONPAGE_H

#include <omkit/section.h>

#include <QWidget>
#include <QDir>

namespace Ui {
class QuestionPage;
}

class QListWidgetItem;
class Solution;

class QuestionPage : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionPage(QWidget *parent = 0);
    ~QuestionPage();

    bool loadCase(const Section& section, const Case& caseValue);
    bool saveAnswer(Solution& solution);
    void connectWith(QListWidgetItem* item);
    bool isAnswered() const;
    bool isModified() const;

public slots:
    void onPageOpened();

signals:
    void enteredAnswer(QListWidgetItem* item);
    void requestedMentorAnswer(QListWidgetItem* item);

private slots:
    void on_answerEdit_textChanged();

    void on_enterButton_clicked();

    void on_toMentorAnswerButton_clicked();

private:
    void updateButtons();

    Ui::QuestionPage *ui;

    Section section;
    Case caseValue;
    QListWidgetItem* item = nullptr;
    bool hasFinalAnswer = false;
};

#endif // QUESTIONPAGE_H
