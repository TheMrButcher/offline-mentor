#ifndef CASEPAGE_H
#define CASEPAGE_H

#include <omkit/case.h>

#include <QWidget>

namespace Ui {
class CasePage;
}

class QTreeWidgetItem;

class CasePage : public QWidget
{
    Q_OBJECT

public:
    explicit CasePage(QWidget *parent = 0);
    ~CasePage();

    Case getCase() const;
    void setCase(const Case& caseValue);
    void connectWith(QTreeWidgetItem* treeItem);
    QTreeWidgetItem* key() const;

signals:
    void nameChanged();
    void removeRequested();
    void questionEditRequested();
    void answerEditRequested();

private slots:
    void on_nameEdit_textEdited(const QString &arg);

    void on_removeButton_clicked();

    void on_editQuestionButton_clicked();

    void on_editAnswerButton_clicked();

private:
    Ui::CasePage *ui;
    Case originalCase;
    QTreeWidgetItem* treeItem = nullptr;
};

#endif // CASEPAGE_H
