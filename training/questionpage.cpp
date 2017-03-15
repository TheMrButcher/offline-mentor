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
    return true;
}
