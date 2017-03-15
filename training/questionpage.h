#ifndef QUESTIONPAGE_H
#define QUESTIONPAGE_H

#include <omkit/section.h>

#include <QWidget>

namespace Ui {
class QuestionPage;
}

class QuestionPage : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionPage(QWidget *parent = 0);
    ~QuestionPage();

    bool loadCase(const Section& section, const Case& caseValue);

private:
    Ui::QuestionPage *ui;

    Section section;
    Case caseValue;
};

#endif // QUESTIONPAGE_H
