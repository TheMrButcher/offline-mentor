#include "mainwindow.h"
#include "sectionsform.h"
#include "sectioneditform.h"
#include "createsectiondialog.h"
#include "settings.h"
#include "section_utils.h"
#include "ui_mainwindow.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/omkit.h>

#include <QFileDialog>
#include <QMessageBox>
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

    createSectionDialog = new CreateSectionDialog(this);
    createSectionDialog->hide();
    connect(createSectionDialog, SIGNAL(accepted()), this, SLOT(onSectionCreated()));

    connect(sectionsForm, SIGNAL(requestedCreation()), this, SLOT(startCreation()));
    connect(sectionsForm, SIGNAL(requestedOpen(Section)), this, SLOT(openSection(Section)));

    connect(ui->createAction, SIGNAL(triggered()), this, SLOT(startCreation()));
    connect(ui->openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(save()));

    QTimer::singleShot(0, this, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCreation()
{
    createSectionDialog->show();
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Открытие раздела", Settings::instance().safeLastDirectoryPath(),
                "Файл раздела (*.oms)");
    if (path.isEmpty())
        return;

    Section section;
    if (isKnownSection(path)) {
        section = getSection(path);
    } else {
        section.path = path;
        if (!section.open()) {
            QMessageBox::warning(this, "Не удалось открыть раздел",
                                 "При открытии раздела произошла ошибка. Проверьте правильность "
                                 "указанного пути.");
            return;
        }
        sectionsForm->addSection(section);
    }
    Settings::instance().updateLastDirectoryPath(path);
    openSection(section);
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
    createSectionDialog->initUI();
    sectionsForm->load();
}

void MainWindow::onSectionCreated()
{
    auto section = createSectionDialog->result();
    Settings::instance().updateLastDirectoryPath(section.path);
    sectionsForm->addSection(section);
    openSection(section);
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
