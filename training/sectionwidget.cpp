#include "sectionwidget.h"
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
    ui->descriptionLabel->setText(!section.description.isEmpty()
                                  ? section.description : "отсутствует");
    ui->questionsNumLabel->setText(QString::number(section.cases.size()));
    ui->groupBox->setTitle("Раздел \"" + section.name + "\"");

    int width = std::max(
                ui->groupBox->sizeHint().width(),
                ui->groupBox->minimumWidth());
    ui->groupBox->setFixedWidth(width);
}
