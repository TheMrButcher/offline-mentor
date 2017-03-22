#include "solutionexplorer.h"
#include "section_utils.h"
#include "answerpage.h"
#include "ui_solutionexplorer.h"
#include <omkit/solution.h>
#include <omkit/section.h>

SolutionExplorer::SolutionExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionExplorer)
{
    ui->setupUi(this);
}

SolutionExplorer::~SolutionExplorer()
{
    delete ui;
}

void SolutionExplorer::setSolution(const Solution& solution)
{
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
    ui->userNameLabel->setText("Отвечал: " + solution.userName);
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
