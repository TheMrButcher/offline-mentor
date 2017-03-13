#include "mainwindow.h"
#include "version.h"
#include "sectionsform.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Offline-наставник. Редактор v." + getVersion());

    sectionsForm = new SectionsForm(this);
    ui->stackedWidget->addWidget(sectionsForm);
    select(sectionsForm);

    Section section;
    QList<Section> sections;

    section.directoryPath = "путь/к/секции";
    section.name = "Имя секции";
    sections.append(section);

    section.directoryPath = "очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/очень/длинный/путь";
    section.name = "Имя секции 2";
    sections.append(section);

    sectionsForm->setSections(sections);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
