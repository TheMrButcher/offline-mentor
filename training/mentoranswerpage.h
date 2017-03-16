#ifndef MENTORANSWERPAGE_H
#define MENTORANSWERPAGE_H

#include <omkit/section.h>

#include <QWidget>

namespace Ui {
class MentorAnswerPage;
}

class QListWidgetItem;

class MentorAnswerPage : public QWidget
{
    Q_OBJECT

public:
    explicit MentorAnswerPage(QWidget *parent = 0);
    ~MentorAnswerPage();

    bool loadCase(const Section& section, const Case& caseValue);
    void connectWith(QListWidgetItem* item);

signals:
    void requestedBack(QListWidgetItem* item);
    void requestedNext(QListWidgetItem* item);

private slots:
    void on_backButton_clicked();

    void on_nextButton_clicked();

private:
    Ui::MentorAnswerPage *ui;

    Section section;
    Case caseValue;
    QListWidgetItem* item = nullptr;
};

#endif // MENTORANSWERPAGE_H
