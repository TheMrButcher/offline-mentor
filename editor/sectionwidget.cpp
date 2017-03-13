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
    return section.directoryPath;
}

void SectionWidget::setSection(const Section& section)
{
    this->section = section;
    ui->directoryPathLabel->setText(section.directoryPath);
    ui->groupBox->setTitle("Секция \"" + section.name + "\"");

    int width = std::max(
                ui->groupBox->sizeHint().width(),
                ui->groupBox->minimumWidth());
    ui->groupBox->setFixedWidth(width);
}
