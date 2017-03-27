#include "solutionsform.h"
#include "solution_utils.h"
#include "section_utils.h"
#include "ui_solutionsform.h"
#include <QMessageBox>
#include <QDebug>

namespace {
QString makeStatistics(const Section& section, const Solution& solution)
{
    int casesNum = section.cases.size();
    int answersNum = solution.answers.size();
    int percent = casesNum > answersNum
            ? static_cast<int>(100.0f * answersNum / casesNum + 0.5f)
            : 100;
    return QString("%1 из %2 (%3\%)").arg(answersNum).arg(casesNum).arg(percent);
}

const QString NO_FILTER_TEXT = "* Не фильтровать *";
const int NO_FILTER_INDEX = 0;
}

SolutionsForm::SolutionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionsForm)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(3, 100);

    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(reload()));
    connect(ui->applyFilterButton, SIGNAL(clicked()), this, SLOT(applyFilter()));
    connect(ui->tableWidget->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(onSelectionChanged(QItemSelection,QItemSelection)));
}

SolutionsForm::~SolutionsForm()
{
    delete ui;
}

void SolutionsForm::reload()
{
    while (ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(ui->tableWidget->rowCount() - 1);
    loadSections();
    loadSolutions();

    updateComboBox(ui->sectionNameComboBox, getSectionNames());
    updateComboBox(ui->userNameComboBox, getUserNames());

    applyFilter();
}

void SolutionsForm::applyFilter()
{
    const auto& allSolutions = getSolutions();
    int sectionIndex = ui->sectionNameComboBox->currentIndex();
    int userIndex = ui->userNameComboBox->currentIndex();
    if (sectionIndex == NO_FILTER_INDEX && userIndex == NO_FILTER_INDEX)
        fillTable(allSolutions);

    const auto& sections = getSections();
    QString sectionName = ui->sectionNameComboBox->currentText();
    QString userName = ui->userNameComboBox->currentText();
    QList<Solution> filteredSolutions;
    filteredSolutions.reserve(allSolutions.size());
    foreach (const auto& solution, allSolutions) {
        if (sectionIndex != NO_FILTER_INDEX
            && sectionName != sections[solution.sectionId].name)
            continue;
        if (userIndex != NO_FILTER_INDEX
            && userName != solution.userName)
            continue;
        filteredSolutions.append(solution);
    }
    fillTable(filteredSolutions);
}

void SolutionsForm::onSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    updateButtons();
}

void SolutionsForm::on_resetFilterButton_clicked()
{
    ui->sectionNameComboBox->setCurrentIndex(NO_FILTER_INDEX);
    ui->userNameComboBox->setCurrentIndex(NO_FILTER_INDEX);
    applyFilter();
}

void SolutionsForm::on_selectSectionButton_clicked()
{
    int row = ui->tableWidget->selectionModel()->selectedRows()[0].row();
    ui->sectionNameComboBox->setCurrentText(ui->tableWidget->item(row, 1)->text());
    applyFilter();
}

void SolutionsForm::on_selectUserButton_clicked()
{
    int row = ui->tableWidget->selectionModel()->selectedRows()[0].row();
    ui->userNameComboBox->setCurrentText(ui->tableWidget->item(row, 2)->text());
    applyFilter();
}

void SolutionsForm::on_openButton_clicked()
{
    int row = ui->tableWidget->selectionModel()->selectedRows()[0].row();
    auto userName = ui->tableWidget->item(row, 2)->text();
    auto sectionId = ui->tableWidget->item(row, 1)->data(Qt::UserRole).toUuid();
    const auto& solution = getSolution(userName, sectionId);
    if (!solution.isValid()) {
        QMessageBox::warning(this, "Ошибка при открытии",
                             "Невозможно открыть выбранное решение.");
        return;
    }
    emit requestedOpen(getSolution(userName, sectionId));
}

void SolutionsForm::fillTable(const QList<Solution>& solutions)
{
    ui->tableWidget->selectionModel()->clearSelection();
    const auto& sections = getSections();
    ui->tableWidget->setRowCount(solutions.size());

    ui->tableWidget->setSortingEnabled(false);
    for (int i = 0; i < solutions.size(); ++i) {
        const auto& solution = solutions[i];
        const auto& section = sections[solution.sectionId];

        qDebug() << "Section: " << section.name << ", user: " << solution.userName
                 << ", answered: " << solution.answers.size() << " / " << section.cases.size();

        QTableWidgetItem* statusItem = new QTableWidgetItem();
        if (solution.answers.size() == section.cases.size()) {
            statusItem->setIcon(QIcon(":/icons/answered.png"));
            statusItem->setText("Завершен");
        } else {
            statusItem->setIcon(QIcon(":/icons/in_progress.png"));
            statusItem->setText("В процессе");
        }
        ui->tableWidget->setItem(i, 0, statusItem);

        QTableWidgetItem* sectionNameItem = new QTableWidgetItem();
        sectionNameItem->setText(section.name);
        sectionNameItem->setData(Qt::UserRole, section.id);
        ui->tableWidget->setItem(i, 1, sectionNameItem);

        QTableWidgetItem* userNameItem = new QTableWidgetItem();
        userNameItem->setText(solution.userName);
        ui->tableWidget->setItem(i, 2, userNameItem);

        QTableWidgetItem* statisticsItem = new QTableWidgetItem();
        statisticsItem->setText(makeStatistics(section, solution));
        statisticsItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(i, 3, statisticsItem);
    }
    ui->tableWidget->setSortingEnabled(true);
}

void SolutionsForm::updateComboBox(QComboBox* comboBox, const QStringList& variants)
{
    int currentIndex = comboBox->currentIndex();
    QString currentText = comboBox->currentText();
    comboBox->clear();
    comboBox->addItem(NO_FILTER_TEXT);
    comboBox->addItems(variants);

    if (currentIndex <= NO_FILTER_INDEX) {
        comboBox->setCurrentIndex(NO_FILTER_INDEX);
    } else {
        int i = 0;
        for (; i < variants.size(); ++i)
            if (currentText == variants[i])
                break;
        if (i == variants.size()) {
            comboBox->setCurrentIndex(NO_FILTER_INDEX);
        } else {
            comboBox->setCurrentIndex(i + 1);
        }
    }
}

void SolutionsForm::updateButtons()
{
    bool isRowSelected = ui->tableWidget->selectionModel()->selectedRows().size() == 1;
    ui->selectSectionButton->setEnabled(isRowSelected);
    ui->selectUserButton->setEnabled(isRowSelected);
    ui->openButton->setEnabled(isRowSelected);
}
