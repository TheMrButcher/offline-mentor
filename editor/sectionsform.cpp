#include "sectionsform.h"
#include "sectionwidget.h"
#include "createsectiondialog.h"
#include "settings.h"
#include "section_utils.h"
#include "ui_sectionsform.h"
#include <QVBoxLayout>
#include <QFileInfo>

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

void SectionsForm::load()
{
    loadSections();
    updateView();
}

void SectionsForm::addSection(const Section& section)
{
    ::addSection(section);
    updateView();
}

void SectionsForm::updateSection(const Section& section)
{
    ::updateSection(section);
    widgets[section.id]->setSection(section);
}

void SectionsForm::on_createSectionButton_clicked()
{
    emit requestedCreation();
}

void SectionsForm::updateView()
{
    widgets.clear();
    qDeleteAll(ui->sectionsList->children());
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    const auto& sections = getSections();
    foreach (const auto& section, sections) {
        SectionWidget* sectionWidget = new SectionWidget;
        sectionWidget->setSection(section);
        layout->addWidget(sectionWidget);
        widgets[section.id] = sectionWidget;
        connect(sectionWidget, SIGNAL(requestedOpen(Section)), this, SIGNAL(requestedOpen(Section)));
    }
    ui->sectionsList->setLayout(layout);
}
