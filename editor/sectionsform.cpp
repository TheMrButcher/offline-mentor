#include "sectionsform.h"
#include "sectionwidget.h"
#include "createsectiondialog.h"
#include "settings.h"
#include "ui_sectionsform.h"
#include <QVBoxLayout>
#include <QFileInfo>

SectionsForm::SectionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionsForm)
{
    ui->setupUi(this);

    createSectionDialog = new CreateSectionDialog(this);
    createSectionDialog->hide();

    connect(createSectionDialog, SIGNAL(accepted()), this, SLOT(onSectionCreated()));
}

SectionsForm::~SectionsForm()
{
    delete ui;
}

void SectionsForm::setSections(QList<Section> sections)
{
    this->sections = sections;
    updateView();
}

void SectionsForm::onSectionCreated()
{
    Section section = createSectionDialog->result();

    auto& settings = Settings::instance();
    settings.lastDirectoryPath = QFileInfo(section.path).absolutePath();
    settings.knownSections.prepend(section.path);
    settings.write();

    sections.prepend(section);
    updateView();
}

void SectionsForm::on_createSectionButton_clicked()
{
    createSectionDialog->show();
}

void SectionsForm::updateView()
{
    qDeleteAll(ui->sectionsList->children());
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(20);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    foreach (const auto& section, sections) {
        SectionWidget* sectionWidget = new SectionWidget;
        sectionWidget->setSection(section);
        layout->addWidget(sectionWidget);
        connect(sectionWidget, SIGNAL(requestedOpen(Section)), this, SIGNAL(requestedOpen(Section)));
    }
    ui->sectionsList->setLayout(layout);
}
