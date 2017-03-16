#include "questionpage.h"
#include "ui_questionpage.h"
#include <omkit/html_utils.h>

QuestionPage::QuestionPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionPage)
{
    ui->setupUi(this);
}

QuestionPage::~QuestionPage()
{
    delete ui;
}

bool QuestionPage::loadCase(const Section& section, const Case& caseValue)
{
    QDir sectionDir = section.dir();
    QString path = sectionDir.absoluteFilePath(caseValue.questionFileName);
    QString html = readHTML(path);
    if (html.isEmpty())
        return false;
    ui->questionBrowser->setHtml(html);
    ui->titleLabel->setText("Кейс \"" + caseValue.name + "\"");
    this->section = section;
    this->caseValue = caseValue;
    ui->enterButton->setEnabled(false);
    ui->toMentorAnswerButton->hide();
    return true;
}

void QuestionPage::connectWith(QListWidgetItem* item)
{
    this->item = item;
}

void QuestionPage::on_answerEdit_textChanged()
{
    ui->enterButton->setEnabled(!ui->answerEdit->document()->isEmpty());
}

void QuestionPage::on_enterButton_clicked()
{
    ui->answerEdit->setReadOnly(true);
    ui->enterButton->hide();
    ui->toMentorAnswerButton->show();
    emit enteredAnswer(item);
}

void QuestionPage::on_toMentorAnswerButton_clicked()
{
    emit requestedMentorAnswer(item);
}
