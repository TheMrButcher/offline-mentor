#include "mentoranswerpage.h"
#include "ui_mentoranswerpage.h"

#include <omkit/html_utils.h>

MentorAnswerPage::MentorAnswerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MentorAnswerPage)
{
    ui->setupUi(this);
}

MentorAnswerPage::~MentorAnswerPage()
{
    delete ui;
}

bool MentorAnswerPage::loadCase(const Section& section, const Case& caseValue)
{
    QDir sectionDir = section.dir();
    QString path = sectionDir.absoluteFilePath(caseValue.answerFileName);
    QString html = readHTML(path);
    if (html.isEmpty())
        return false;
    setImageAndHTML(section.dir(), caseValue.answerImage,
                    html, ui->answerBrowser);
    ui->titleLabel->setText("Кейс \"" + caseValue.name + "\". Ответ наставника");
    this->section = section;
    this->caseValue = caseValue;
    return true;
}

void MentorAnswerPage::connectWith(QListWidgetItem* item)
{
    this->item = item;
}

void MentorAnswerPage::on_backButton_clicked()
{
    emit requestedBack(item);
}

void MentorAnswerPage::on_nextButton_clicked()
{
    emit requestedNext(item);
}
