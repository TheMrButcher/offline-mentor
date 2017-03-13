#include "sectioneditform.h"
#include "ui_sectioneditform.h"

SectionEditForm::SectionEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionEditForm)
{
    ui->setupUi(this);
}

SectionEditForm::~SectionEditForm()
{
    delete ui;
}

Section SectionEditForm::section() const
{
    Section result = sectionFromUI();
    result.path = originalSection.path;
    return result;
}

void SectionEditForm::setSection(const Section& section)
{
    this->originalSection = section;

    ui->nameEdit->setText(section.name);
    ui->descriptionEdit->setPlainText(section.description);
}

Section SectionEditForm::sectionFromUI() const
{
    Section section;
    section.name = ui->nameEdit->text();
    section.description = ui->descriptionEdit->toPlainText();
    return section;
}
