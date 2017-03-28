#include "trainingform.h"
#include "questionpage.h"
#include "mentoranswerpage.h"
#include "totalpage.h"
#include "solution_utils.h"
#include "settings.h"
#include "ui_trainingform.h"
#include <QMessageBox>

TrainingForm::TrainingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingForm)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);
}

TrainingForm::~TrainingForm()
{
    delete ui;
}

bool TrainingForm::setSection(const Section& section)
{
    if (!section.isValid())
        return false;

    ui->listWidget->clear();
    for(int i = ui->stackedWidget->count() - 1; i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    nodes.clear();

    this->section = section;
    ui->titleLabel->setText("Раздел \"" + section.name + "\"");
    int nextCaseIndex = 1;
    QStringList badFiles;

    QListWidgetItem* prevItem = nullptr;
    foreach (const auto& caseValue, section.cases) {
        QuestionPage* questionPage = new QuestionPage;
        if (!questionPage->loadCase(section, caseValue)) {
            delete questionPage;
            badFiles.append(caseValue.name + "/Вопрос");
            continue;
        }

        MentorAnswerPage* mentorAnswerPage = new MentorAnswerPage;
        if (!mentorAnswerPage->loadCase(section, caseValue)) {
            delete questionPage;
            delete mentorAnswerPage;
            badFiles.append(caseValue.name + "/Ответ");
            continue;
        }

        int questionPageId = ui->stackedWidget->addWidget(questionPage);
        int mentorAnswerPageId = ui->stackedWidget->addWidget(mentorAnswerPage);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setText(QString("%1. Кейс \"%2\"").arg(nextCaseIndex).arg(caseValue.name));
        if (questionPage->isAnswered()) {
            item->setIcon(QIcon(":/icons/answered.png"));
        } else {
            item->setIcon(QIcon(":/icons/question.png"));
        }
        nodes[item] = NodeDescriptor{ questionPageId, mentorAnswerPageId, nullptr };
        if (prevItem)
            nodes[prevItem].nextItem = item;

        questionPage->connectWith(item);
        connect(questionPage, SIGNAL(enteredAnswer(QListWidgetItem*)),
                this, SLOT(onAnswerEntered(QListWidgetItem*)));
        connect(questionPage, SIGNAL(requestedMentorAnswer(QListWidgetItem*)),
                this, SLOT(toMentorAnswer(QListWidgetItem*)));

        mentorAnswerPage->connectWith(item);
        connect(mentorAnswerPage, SIGNAL(requestedBack(QListWidgetItem*)),
                this, SLOT(backToQuestion(QListWidgetItem*)));
        connect(mentorAnswerPage, SIGNAL(requestedNext(QListWidgetItem*)),
                this, SLOT(next(QListWidgetItem*)));

        if (!firstCaseItem)
            firstCaseItem = item;

        ++nextCaseIndex;
        prevItem = item;
    }

    if (!firstCaseItem)
        return false;

    ui->listWidget->setCurrentItem(firstCaseItem);
    openQuestionPage(nodes[firstCaseItem].questionPageId);

    if (isSectionCompleted()) {
        updateTotal();
        ui->listWidget->setCurrentItem(totalItem);
        ui->stackedWidget->setCurrentWidget(totalPage);
    }

    if (!badFiles.isEmpty()) {
        QMessageBox::warning(this, "Ошибка при загрузке",
                             "Не удалось загрузить некоторые файлы кейсов: " + badFiles.join("; ")
                             + ". Они не будут показаны в списке кейсов.");
    }
    return true;
}

QUuid TrainingForm::sectionId() const
{
    return section.id;
}

void TrainingForm::on_listWidget_itemSelectionChanged()
{
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty())
        return;
    auto item = selectedItems.front();
    if (totalItem && item == totalItem) {
        ui->stackedWidget->setCurrentWidget(totalPage);
        return;
    }
    openQuestionPage(nodes[item].questionPageId);
}

void TrainingForm::onAnswerEntered(QListWidgetItem* caseItem)
{
    if (!nodes.contains(caseItem))
        return;
    const auto& node = nodes[caseItem];
    auto questionPage = (QuestionPage*)ui->stackedWidget->widget(node.questionPageId);
    Solution solution = getSolution(SolutionPathType::Local, section);
    bool isSavedLocally = false;
    if (solution.isValid()
        && questionPage->saveAnswer(solution)
        && saveSolution(SolutionPathType::Local, solution)) {
        caseItem->setIcon(QIcon(":/icons/answered.png"));
        isSavedLocally = true;
        emit savedSolution(solution);
    } else {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось сохранить ответ локально. "
                             "Возможно, приложение настроено неверно.");
    }

    if (Settings::instance().hasRemoteSolutionsDir) {
        solution = getSolution(SolutionPathType::Remote, section);
        if (!solution.isValid()
            || !questionPage->saveAnswer(solution)
            || !saveSolution(SolutionPathType::Remote, solution)) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Не удалось сохранить ответ на сервере. "
                                 "Нет доступа к папке для ответов."
                                 + (isSavedLocally ? QString(" Ответ был сохранен локально.") : ""));
        }
    }

    if (isSavedLocally) {
        if (isSectionCompleted())
            updateTotal();
    }
    ui->stackedWidget->setCurrentIndex(node.mentorAnswerPageId);
}

void TrainingForm::toMentorAnswer(QListWidgetItem* caseItem)
{
    if (!nodes.contains(caseItem))
        return;
    ui->stackedWidget->setCurrentIndex(nodes[caseItem].mentorAnswerPageId);
}

void TrainingForm::backToQuestion(QListWidgetItem* caseItem)
{
    if (!nodes.contains(caseItem))
        return;
    openQuestionPage(nodes[caseItem].questionPageId);
}

void TrainingForm::next(QListWidgetItem* caseItem)
{
    if (!nodes.contains(caseItem))
        return;
    const auto& node = nodes[caseItem];
    if (node.nextItem) {
        ui->listWidget->setCurrentItem(node.nextItem);
        openQuestionPage(nodes[node.nextItem].questionPageId);
    } else {
        if (isSectionCompleted()) {
            ui->listWidget->setCurrentItem(totalItem);
            ui->stackedWidget->setCurrentWidget(totalPage);
        } else {
            ui->listWidget->setCurrentItem(firstCaseItem);
            openQuestionPage(nodes[firstCaseItem].questionPageId);
        }
    }
}

void TrainingForm::transferSolution()
{
    Solution localSolution = getSolution(SolutionPathType::Local, section);
    Solution remoteSolution = getSolution(SolutionPathType::Remote, section);
    if (remoteSolution.isValid()
        && mergeSolution(localSolution, SolutionPathType::Remote, remoteSolution)) {
        totalPage->setSuccess();
    } else {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось сохранить ответ на сервере. "
                             "Нет доступа к папке для ответов.");
    }
}

void TrainingForm::openQuestionPage(int pageId)
{
    QuestionPage* page = (QuestionPage*)ui->stackedWidget->widget(pageId);
    page->onPageOpened();
    ui->stackedWidget->setCurrentIndex(pageId);
}

bool TrainingForm::isSectionCompleted() const
{
    Solution solution = getSolution(SolutionPathType::Local, section);
    if (!solution.isValid())
        return false;
    return solution.answers.size() == section.cases.size();
}

void TrainingForm::updateTotal()
{
    if (!totalPage) {
        totalPage = new TotalPage;
        totalPage->load(section);
        ui->stackedWidget->addWidget(totalPage);
        connect(totalPage, SIGNAL(requestedTransfer()), this, SLOT(transferSolution()));

        totalItem = new QListWidgetItem(ui->listWidget);
        totalItem->setText("Итоги раздела");
        totalItem->setIcon(QIcon(":/icons/total.png"));
    }

    if (Settings::instance().hasRemoteSolutionsDir) {
        Solution localSolution = getSolution(SolutionPathType::Local, section);
        Solution remoteSolution = getSolution(SolutionPathType::Remote, section);
        if (remoteSolution.isValid()) {
            if (localSolution.answers.size() == remoteSolution.answers.size()) {
                totalPage->setSuccess();
            } else {
                if (mergeSolution(localSolution, SolutionPathType::Remote, remoteSolution)) {
                    totalPage->setSuccess();
                } else {
                    totalPage->setTransferError();
                }
            }
        } else {
            totalPage->setTransferError();
        }
    } else {
        totalPage->setUnknownState();
    }
}
