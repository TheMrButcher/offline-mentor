#include "sectionwidget.h"
#include "solution_utils.h"
#include "ui_sectionwidget.h"

SectionWidget::SectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionWidget)
{
    ui->setupUi(this);
}

SectionWidget::~SectionWidget()
{
    delete ui;
}

void SectionWidget::setSection(const Section& section)
{
    this->section = section;
    ui->descriptionLabel->setText(!section.description.isEmpty()
                                  ? section.description : "отсутствует");
    ui->questionsNumLabel->setText(QString::number(section.cases.size()));
    ui->groupBox->setTitle("Раздел \"" + section.name + "\"");

    ui->descriptionLabel->setFixedHeight(ui->descriptionLabel->sizeHint().height());
    int width = std::max(
                ui->groupBox->sizeHint().width(),
                ui->groupBox->minimumWidth());
    ui->groupBox->setFixedWidth(width);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(section.cases.size());

    updateProgress();
}

void SectionWidget::updateProgress()
{
    if (!hasSolution(SolutionPathType::Local, section))
        return;
    Solution solution = getSolution(SolutionPathType::Local, section);
    if (solution.isValid())
        ui->progressBar->setValue(solution.answers.size());
}

void SectionWidget::on_enterButton_clicked()
{
    emit requestedOpen(section);
}
