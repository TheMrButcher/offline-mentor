#include "mainwindow.h"
#include "sectionsform.h"
#include "sectioneditform.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
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
    int tabIndex = ui->tabWidget->addTab(sectionsForm, "Список разделов");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);

    ui->tabWidget->setCurrentWidget(ui->tabWidget);

    connect(sectionsForm, SIGNAL(requestedOpen(Section)), this, SLOT(openSection(Section)));

    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(save()));

    QTimer::singleShot(0, this, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save()
{
    if (isSectionsFormCurrent())
        return;
    ((SectionEditForm*)ui->tabWidget->currentWidget())->save();
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
    if (openedPages.contains(section.id)) {
        ui->tabWidget->setCurrentWidget(openedPages[section.id]);
        return;
    }
    SectionEditForm* sectionEditForm = new SectionEditForm(this);
    sectionEditForm->setSection(section);
    ui->tabWidget->addTab(sectionEditForm, trim(section.name, 16));
    ui->tabWidget->setCurrentWidget(sectionEditForm);
    openedPages[section.id] = sectionEditForm;
    connect(sectionEditForm, SIGNAL(sectionSaved(Section)),
            this, SLOT(onSectionSaved(Section)));
}

void MainWindow::onSectionSaved(const Section& section)
{
    auto widget = openedPages[section.id];
    int index = ui->tabWidget->indexOf(widget);
    ui->tabWidget->setTabText(index, trim(section.name, 16));
    sectionsForm->updateSection(section);
}

bool MainWindow::isSectionsFormCurrent() const
{
    return ui->tabWidget->currentWidget() == sectionsForm;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index == ui->tabWidget->indexOf(sectionsForm))
        return;
    auto widget = ui->tabWidget->widget(index);
    auto id = ((SectionEditForm*)widget)->sectionId();
    openedPages.remove(id);
    delete widget;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    bool isEditor = index != ui->tabWidget->indexOf(sectionsForm);
    ui->saveAction->setEnabled(isEditor);
}
