#include "answerpage.h"
#include "textexplorer.h"
#include "ui_answerpage.h"
#include <omkit/case.h>
#include <omkit/section.h>
#include <omkit/solution.h>
#include <omkit/html_utils.h>

AnswerPage::AnswerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnswerPage)
{
    ui->setupUi(this);
}

AnswerPage::~AnswerPage()
{
    delete ui;
}

AnswerStatus AnswerPage::load(
        const Section& section, const Solution& solution, int caseIndex)
{
    this->caseIndex = caseIndex;
    const Case& caseValue = section.cases[caseIndex];
    ui->titleLabel->setText("Кейс \"" + caseValue.name + "\"");
    if (caseIndex == 0)
        ui->prevButton->setEnabled(false);
    if (caseIndex == section.cases.size() - 1)
        ui->nextButton->setEnabled(false);

    auto sectionDir = section.dir();
    bool hasErrors = false;
    auto questionHTML = readHTML(sectionDir.absoluteFilePath(caseValue.questionFileName));
    if (questionHTML.isEmpty()) {
        hasErrors = true;
        ui->questionBrowser->setPlainText("Произошла ошибка при загрузке текста вопроса.");
    } else {
        setImageAndHTML(section.dir(), caseValue.questionImage,
                        questionHTML, ui->questionBrowser);
    }

    TextExplorer* answerExplorer = new TextExplorer(this);
    answerExplorer->setTitle("Ответ пользователя");
    Answer answer = solution.answer(caseValue);
    bool hasFinalAnswer = answer.isFinal();
    if (hasFinalAnswer) {
        if (!answerExplorer->load(
                solution.dir().absoluteFilePath(answer.fileName))) {
            hasErrors = true;
            answerExplorer->setPlainText("Произошла ошибка при загрузке ответа пользователя.");
        }
    } else {
        answerExplorer->setPlainText("Пользователь еще не ответил на данный вопрос.");
    }
    ui->tabWidget->addTab(answerExplorer, "Ответ пользователя");

    TextExplorer* mentrorAnswerExplorer = new TextExplorer(this);
    mentrorAnswerExplorer->setTitle("Ответ наставника");
    if (!mentrorAnswerExplorer->load(
            sectionDir, caseValue.answerFileName, caseValue.answerImage)) {
        hasErrors = true;
        mentrorAnswerExplorer->setPlainText("Произошла ошибка при загрузке ответа наставника.");
    }
    ui->tabWidget->addTab(mentrorAnswerExplorer, "Ответ наставника");

    if (hasErrors)
        return AnswerStatus::Error;
    return hasFinalAnswer ? AnswerStatus::OK : AnswerStatus::Absent;
}

void AnswerPage::on_prevButton_clicked()
{
    emit requestedCase(caseIndex - 1);
}

void AnswerPage::on_nextButton_clicked()
{
    emit requestedCase(caseIndex + 1);
}
