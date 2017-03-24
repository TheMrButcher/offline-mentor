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

    ui->descriptionLabel->adjustSize();
    ui->descriptionLabel->setFixedSize(ui->descriptionLabel->sizeHint());
    ui->groupBox->adjustSize();
    ui->groupBox->setFixedSize(ui->groupBox->sizeHint());

    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(section.cases.size());

    updateProgress();
}

void SectionWidget::updateProgress()
{
    if (!hasSolution(SolutionPathType::Local, section))
        return;
    const Solution& solution = peekSolution(SolutionPathType::Local, section);
    if (solution.isValid()) {
        ui->progressBar->setValue(solution.answers.size());
        if (ui->progressBar->maximum() == solution.answers.size()) {
            ui->enterButton->setText("Просмотреть");
            ui->progressBar->setStyleSheet(
                    R"delim(
                    QProgressBar {
                        border: 2px solid grey;
                        border-radius: 5px;
                        text-align: center;
                    }
                    QProgressBar::chunk {
                        background-color: rgb(15, 255, 60);
                        width: 20px;
                    })delim");
        } else {
            ui->enterButton->setText("Продолжить");
        }
    }
}

void SectionWidget::on_enterButton_clicked()
{
    emit requestedOpen(section);
}
