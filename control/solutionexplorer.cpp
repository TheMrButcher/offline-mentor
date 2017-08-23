#include "solutionexplorer.h"
#include "section_utils.h"
#include "answerpage.h"
#include "ui_solutionexplorer.h"
#include "solution_utils.h"
#include <omkit/solution.h>
#include <omkit/section.h>
#include <omkit/username.h>
#include <QInputDialog>
#include <QMessageBox>

SolutionExplorer::SolutionExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionExplorer)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);
}

SolutionExplorer::~SolutionExplorer()
{
    delete ui;
}

void SolutionExplorer::setSolution(const Solution& solution)
{
    sectionId = solution.sectionId;
    userName = solution.userName;

    caseDescriptors.clear();
    ui->listWidget->clear();
    for(int i = ui->stackedWidget->count() - 1; i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    const auto& sections = getSections();
    const auto& section = sections[solution.sectionId];
    ui->titleLabel->setText("Раздел \"" + section.name + "\"");
    ui->userNameLabel->setText(solution.userName);
    for (int caseIndex = 0; caseIndex < section.cases.size(); ++caseIndex) {
        const auto& caseValue = section.cases[caseIndex];
        AnswerPage* page = new AnswerPage(this);
        auto status = page->load(section, solution, caseIndex);
        int pageIndex = ui->stackedWidget->addWidget(page);
        connect(page, SIGNAL(requestedCase(int)), this, SLOT(selectCase(int)));

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, pageIndex);
        item->setText(QString("%1. Кейс \"%2\"").arg(caseIndex + 1).arg(caseValue.name));
        switch (status) {
        case AnswerStatus::OK: item->setIcon(QIcon(":/icons/answered.png")); break;
        case AnswerStatus::Error: item->setIcon(QIcon(":/icons/danger.png")); break;
        case AnswerStatus::Absent: item->setIcon(QIcon(":/icons/question.png")); break;
        }
        if (caseIndex == 0)
            ui->listWidget->setCurrentItem(item);

        caseDescriptors[caseIndex] = Descriptor{ pageIndex, item };
    }
}

void SolutionExplorer::selectCase(int caseIndex)
{
    const auto& descriptor = caseDescriptors[caseIndex];
    ui->listWidget->setCurrentItem(descriptor.item);
}

void SolutionExplorer::on_listWidget_itemSelectionChanged()
{
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty())
        return;
    auto item = selectedItems.front();
    int pageIndex = item->data(Qt::UserRole).toInt();
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void SolutionExplorer::on_editUserNameButton_clicked()
{
    QString newUserName = QInputDialog::getText(this, "Новое имя автора",
                                                "Введите новое имя пользователя", QLineEdit::Normal,
                                                userName);
    if (newUserName.isEmpty())
        return;

    if (!isValidUserName(newUserName)) {
        QMessageBox::warning(this, "Неверные данные", "Имя пользователя может содержать "
                                                      "только латиницу, кириллицу, цифры, "
                                                      "пробельные символы, дефисы и знаки "
                                                      "нижнего подчеркивания.");
        return;
    }

    if (getSolution(newUserName, sectionId).isValid()) {
        QMessageBox::warning(this, "Неверные данные", "Решение с таким именем пользователя и "
                                                      "названием раздела уже существует.");
        return;
    }

    if (!changeSolutionAuthor(userName, sectionId, newUserName)) {
        QMessageBox::warning(this, "Ошибка при перемещении",
                             "При перемещении решения произошла ошибка.");
        return;
    }

    userName = newUserName;
    ui->userNameLabel->setText(userName);
    emit authorRenamed();
}
