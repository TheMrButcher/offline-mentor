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

QString SectionWidget::key() const
{
    return section.path;
}

void SectionWidget::setSection(const Section& section)
{
    this->section = section;
    ui->pathLabel->setText(section.path);
    ui->descriptionLabel->setText(!section.description.isEmpty()
                                  ? section.description : "отсутствует");
    ui->questionsNumLabel->setText(QString::number(section.cases.size()));
    ui->groupBox->setTitle("Раздел \"" + section.name + "\"");

    ui->descriptionLabel->adjustSize();
    ui->descriptionLabel->setFixedSize(ui->descriptionLabel->sizeHint());
    ui->groupBox->adjustSize();
    ui->groupBox->setFixedSize(ui->groupBox->sizeHint());
}

void SectionWidget::on_openButton_clicked()
{
    emit requestedOpen(section);
}
