#ifndef QUESTIONPAGE_H
#define QUESTIONPAGE_H

#include <omkit/section.h>

#include <QWidget>

namespace Ui {
class QuestionPage;
}

class QListWidgetItem;

class QuestionPage : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionPage(QWidget *parent = 0);
    ~QuestionPage();

    bool loadCase(const Section& section, const Case& caseValue);
    void connectWith(QListWidgetItem* item);

signals:
    void enteredAnswer(QListWidgetItem* item);
    void requestedMentorAnswer(QListWidgetItem* item);

private slots:
    void on_answerEdit_textChanged();

    void on_enterButton_clicked();

    void on_toMentorAnswerButton_clicked();

private:
    Ui::QuestionPage *ui;

    Section section;
    Case caseValue;
    QListWidgetItem* item = nullptr;
};

#endif // QUESTIONPAGE_H
