#include "mainwindow.h"
#include "sectionsform.h"
#include "sectioneditform.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <omkit/utils.h>
#include <omkit/omkit.h>

#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Offline-наставник. Редактор v." + getVersion());
    showMaximized();

    qDebug() << "OMKit: v." << OMKit::instance().getVersion();
    OMKit::instance().init();

    sectionsForm = new SectionsForm(this);
    ui->stackedWidget->addWidget(sectionsForm);

    sectionEditForm = new SectionEditForm(this);
    ui->stackedWidget->addWidget(sectionEditForm);

    select(sectionsForm);

    connect(sectionsForm, SIGNAL(requestedOpen(Section)), this, SLOT(openSection(Section)));

    connect(ui->saveAction, SIGNAL(triggered()), sectionEditForm, SLOT(save()));

    QTimer::singleShot(0, this, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    auto& settings = Settings::instance();
    settings.read();

    QList<Section> sections;
    sections.reserve(settings.knownSections.size());
    foreach (const auto& path, settings.knownSections) {
        Section section;
        section.path = path;
        if (section.open()) {
            sections.append(section);
        }
    }
    sectionsForm->setSections(sections);

    settings.knownSections.clear();
    foreach (const auto& section, sections)
        settings.knownSections.append(section.path);
    settings.write();
}

void MainWindow::openSection(const Section& section)
{
    sectionEditForm->setSection(section);
    select(sectionEditForm);
    ui->saveAction->setEnabled(true);
}

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
