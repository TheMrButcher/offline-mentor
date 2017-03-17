#include "questionpage.h"
#include "solution_utils.h"
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
    QString questionHTML = readHTML(sectionDir.absoluteFilePath(caseValue.questionFileName));
    if (questionHTML.isEmpty())
        return false;
    ui->questionBrowser->setHtml(questionHTML);

    Solution solution = getSolution(SolutionPathType::Local, section);
    if (solution.isValid()) {
        Answer answer = solution.answer(caseValue);
        if (answer.isValid()) {
            QDir solutionDir = solution.dir();
            QString answerHTML = readHTML(
                        solutionDir.absoluteFilePath(answer.fileName));
            if (!answerHTML.isEmpty()) {
                ui->answerEdit->setHtml(answerHTML);
                hasFinalAnswer = true;
            }
        }
    }

    ui->titleLabel->setText("Кейс \"" + caseValue.name + "\"");
    this->section = section;
    this->caseValue = caseValue;
    updateButtons();
    return true;
}

bool QuestionPage::saveAnswer(Solution& solution)
{
    Answer answer = Answer::createAnswer(caseValue);
    QDir solutionDir = solution.dir();
    QString answerFileName = solutionDir.absoluteFilePath(answer.fileName);
    if (!writeHTML(answerFileName, ui->answerEdit->document()))
        return false;

    solution.answers.append(answer);
    return true;
}

void QuestionPage::connectWith(QListWidgetItem* item)
{
    this->item = item;
}

bool QuestionPage::isAnswered() const
{
    return hasFinalAnswer;
}

void QuestionPage::onPageOpened()
{
    if (!ui->answerEdit->isReadOnly())
        ui->answerEdit->setFocus();
}

void QuestionPage::on_answerEdit_textChanged()
{
    ui->enterButton->setEnabled(!ui->answerEdit->document()->isEmpty());
}

void QuestionPage::on_enterButton_clicked()
{
    hasFinalAnswer = true;
    updateButtons();
    emit enteredAnswer(item);
}

void QuestionPage::on_toMentorAnswerButton_clicked()
{
    emit requestedMentorAnswer(item);
}

void QuestionPage::updateButtons()
{
    if (hasFinalAnswer) {
        ui->answerEdit->setReadOnly(true);
        ui->enterButton->hide();
        ui->toMentorAnswerButton->show();
    } else {
        ui->enterButton->setEnabled(!ui->answerEdit->document()->isEmpty());
        ui->enterButton->show();
        ui->toMentorAnswerButton->hide();
    }
}
