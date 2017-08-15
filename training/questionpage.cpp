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
    setImageAndHTML(section.dir(), caseValue.questionImage,
                    questionHTML, ui->questionBrowser);

    Solution solution = getSolution(SolutionPathType::Local, section);
    if (solution.isValid()) {
        Answer answer = solution.answer(caseValue);
        if (answer.isValid()) {
            QDir solutionDir = solution.dir();
            QString answerHTML = readHTML(
                        solutionDir.absoluteFilePath(answer.fileName));
            if (!answerHTML.isEmpty()) {
                ui->answerEdit->setHtml(answerHTML);
                ui->answerEdit->document()->setModified(false);
                hasFinalAnswer = answer.isFinal();
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
    Solution solutionCopy = solution;
    Answer& answer = solutionCopy.addAnswer(caseValue);
    if (hasFinalAnswer) {
        answer.markAsFinal();
    } else {
        answer.version++;
    }
    QDir solutionDir = solutionCopy.dir();
    QString answerFileName = solutionDir.absoluteFilePath(answer.fileName);
    if (!writeHTML(answerFileName, ui->answerEdit->document()))
        return false;
    solution = solutionCopy;
    ui->answerEdit->document()->setModified(false);
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

bool QuestionPage::isModified() const
{
    return ui->answerEdit->document()->isModified();
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
