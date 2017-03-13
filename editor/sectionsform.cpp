#include "sectionsform.h"
#include "sectionwidget.h"
#include "ui_sectionsform.h"

SectionsForm::SectionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionsForm)
{
    ui->setupUi(this);
}

SectionsForm::~SectionsForm()
{
    delete ui;
}

void SectionsForm::setSections(QList<Section> sections)
{
    foreach (const auto& section, sections) {
        SectionWidget* sectionWidget = new SectionWidget;
        sectionWidget->setSection(section);
        ui->sectionsList->layout()->addWidget(sectionWidget);
    }
}
