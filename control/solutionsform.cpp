#include "solutionsform.h"
#include "solution_utils.h"
#include "section_utils.h"
#include "ui_solutionsform.h"

namespace {
QString makeStatistics(const Section& section, const Solution& solution)
{
    int casesNum = section.cases.size();
    int answersNum = solution.answers.size();
    int percent = casesNum > answersNum
            ? static_cast<int>(100.0f * answersNum / casesNum + 0.5f)
            : 100;
    return QString("%1 из %2 (%3\%)").arg(answersNum).arg(casesNum).arg(percent);
}
}

SolutionsForm::SolutionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionsForm)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(3, 100);

    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(reload()));
}

SolutionsForm::~SolutionsForm()
{
    delete ui;
}

void SolutionsForm::reload()
{
    while (ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(ui->tableWidget->rowCount() - 1);
    loadSolutions();

    const auto& sections = getSections();
    const auto& solutions = getSolutions();
    ui->tableWidget->setRowCount(solutions.size());

    for (int i = 0; i < solutions.size(); ++i) {
        const auto& solution = solutions[i];
        const auto& section = sections[solution.sectionId];

        QTableWidgetItem* statusItem = new QTableWidgetItem();
        if (solution.answers.size() == section.cases.size()) {
            statusItem->setIcon(QIcon(":/icons/answered.png"));
            statusItem->setText("Завершен");
        } else {
            statusItem->setIcon(QIcon(":/icons/in_progress.png"));
            statusItem->setText("В процессе");
        }
        ui->tableWidget->setItem(i, 0, statusItem);

        QTableWidgetItem* sectionNameItem = new QTableWidgetItem();
        sectionNameItem->setText(section.name);
        ui->tableWidget->setItem(i, 1, sectionNameItem);

        QTableWidgetItem* userNameItem = new QTableWidgetItem();
        userNameItem->setText(solution.userName);
        ui->tableWidget->setItem(i, 2, userNameItem);

        QTableWidgetItem* statisticsItem = new QTableWidgetItem();
        statisticsItem->setText(makeStatistics(section, solution));
        statisticsItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(i, 3, statisticsItem);
    }
}
