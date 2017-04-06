#include "casepage.h"
#include "ui_casepage.h"

#include <QTreeWidgetItem>

CasePage::CasePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CasePage)
{
    ui->setupUi(this);
}

CasePage::~CasePage()
{
    delete ui;
}

Case CasePage::getCase() const
{
    Case result = originalCase;
    result.name = ui->nameEdit->text();
    return result;
}

void CasePage::setCase(const Case& caseValue)
{
    originalCase = caseValue;
    ui->nameEdit->setText(caseValue.name);
}

void CasePage::connectWith(QTreeWidgetItem* treeItem)
{
    this->treeItem = treeItem;
}

QTreeWidgetItem* CasePage::key() const
{
    return treeItem;
}

void CasePage::on_nameEdit_textEdited(const QString &arg)
{
    if (!treeItem)
        return;
    treeItem->setText(0, "Кейс \"" + arg + "\"");
    emit nameChanged();
}

void CasePage::on_removeButton_clicked()
{
    emit removeRequested();
}

void CasePage::on_editQuestionButton_clicked()
{
    emit questionEditRequested();
}

void CasePage::on_editAnswerButton_clicked()
{
    emit answerEditRequested();
}
