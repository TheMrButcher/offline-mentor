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

void SectionsForm::setUserName(QString name)
{
    ui->greetingsLabel->setText("Здравствуйте, " + name + "!");
}

void SectionsForm::setSections(QList<Section> sections)
{
    sectionWidgets.clear();
    foreach (const auto& section, sections) {
        SectionWidget* widget = new SectionWidget(this);
        widget->setSection(section);
        sectionWidgets.append(widget);
        ui->sectionsList->layout()->addWidget(widget);
        connect(widget, SIGNAL(requestedOpen(Section)), this, SIGNAL(requestedOpen(Section)));
    }
}

void SectionsForm::updateProgress()
{
    foreach (SectionWidget* widget, sectionWidgets)
        widget->updateProgress();
}
