#include "mainwindow.h"
#include "version.h"
#include "sectionsform.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Offline-наставник. Редактор v." + getVersion());

    sectionsForm = new SectionsForm(this);
    ui->stackedWidget->addWidget(sectionsForm);
    select(sectionsForm);

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

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
