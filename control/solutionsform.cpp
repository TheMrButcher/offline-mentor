#include "solutionsform.h"
#include "solution_utils.h"
#include "section_utils.h"
#include "group_utils.h"
#include "ui_solutionsform.h"
#include <QMessageBox>

namespace {
QString makeStatistics(const Section& section, const Solution& solution)
{
    int casesNum = section.cases.size();
    int answersNum = solution.finalAnswersNum();
    int percent = casesNum > answersNum
            ? static_cast<int>(100.0f * answersNum / casesNum + 0.5f)
            : 100;
    return QString("%1 из %2 (%3\%)").arg(answersNum).arg(casesNum).arg(percent);
}

const QString NO_FILTER_TEXT = "* Не фильтровать *";
const int NO_FILTER_INDEX = 0;
const QString NO_GROUP_TEXT = "* Только без группы *";
const int NO_GROUP_INDEX = 1;
}

SolutionsForm::SolutionsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionsForm)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(4, 100);

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
    if (ui->groupNameComboBox->count() == 0)
        updateGroupComboBox();
    selectGroupComboVariant(ui->groupNameComboBox->currentIndex());
    applyFilter();
}

void SolutionsForm::onGroupCollectionChanged()
{
    updateGroupComboBox();
    selectGroupComboVariant(ui->groupNameComboBox->currentIndex());
    applyFilter();
}

void SolutionsForm::applyFilter()
{
    const auto& allSolutions = getSolutions();
    int sectionIndex = ui->sectionNameComboBox->currentIndex();
    int userIndex = ui->userNameComboBox->currentIndex();
    int groupIndex = ui->groupNameComboBox->currentIndex();
    if (sectionIndex == NO_FILTER_INDEX
        && userIndex == NO_FILTER_INDEX
        && groupIndex == NO_FILTER_INDEX)
        fillTable(allSolutions);

    const auto& sections = getSections();
    QString sectionName = ui->sectionNameComboBox->currentText();
    QString userName = ui->userNameComboBox->currentText();
    const auto& group = getGroup(ui->groupNameComboBox->currentData().toUuid());
    QList<Solution> filteredSolutions;
    filteredSolutions.reserve(allSolutions.size());
    foreach (const auto& solution, allSolutions) {
        if (sectionIndex != NO_FILTER_INDEX
            && sectionName != sections[solution.sectionId].name)
            continue;
        if (userIndex == NO_FILTER_INDEX) {
            if (groupIndex != NO_FILTER_INDEX) {
                if (groupIndex == NO_GROUP_INDEX) {
                    if (!getGroupsByUserName(solution.userName).empty())
                        continue;
                } else {
                    if (!group.userNames.contains(solution.userName))
                        continue;
                }
            }
        } else {
            if (userName != solution.userName)
                continue;
        }
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
    ui->groupNameComboBox->setCurrentIndex(NO_FILTER_INDEX);
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
    openSolutionInRow(row);
}

void SolutionsForm::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    openSolutionInRow(index.row());
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

        QTableWidgetItem* statusItem = new QTableWidgetItem();
        if (solution.finalAnswersNum() == section.cases.size()) {
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

        QTableWidgetItem* groupNameItem = new QTableWidgetItem();
        const auto& groups = getGroupsByUserName(solution.userName);
        QUuid groupId;
        if (groups.size() == 1) {
            groupId = groups.first()->id;
            groupNameItem->setText(groups.first()->name);
        } else if (groups.size() > 1) {
            QStringList groupNames;
            groupNames.reserve(groups.size());
            for (const auto& group : groups)
                groupNames.append(group->name);
            groupNameItem->setText(groupNames.join(", "));
        }
        groupNameItem->setData(Qt::UserRole, groupId);
        ui->tableWidget->setItem(i, 3, groupNameItem);

        QTableWidgetItem* statisticsItem = new QTableWidgetItem();
        statisticsItem->setText(makeStatistics(section, solution));
        statisticsItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(i, 4, statisticsItem);
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

void SolutionsForm::updateGroupComboBox()
{
    isGroupComboBoxReady = false;
    auto comboBox = ui->groupNameComboBox;
    int currentIndex = comboBox->currentIndex();
    QUuid currentId = comboBox->currentData().toUuid();
    comboBox->clear();
    comboBox->addItem(NO_FILTER_TEXT);
    comboBox->addItem(NO_GROUP_TEXT);
    const auto& groups = getGroups();
    for (const auto& group : groups)
        comboBox->addItem(group.name, group.id);
    if (currentIndex <= NO_FILTER_INDEX) {
        comboBox->setCurrentIndex(NO_FILTER_INDEX);
    } else if (currentIndex == NO_GROUP_INDEX){
        comboBox->setCurrentIndex(NO_GROUP_INDEX);
    } else {
        int i = 0;
        for (; i < groups.size(); ++i)
            if (currentId == groups[i].id)
                break;
        if (i == groups.size()) {
            comboBox->setCurrentIndex(NO_FILTER_INDEX);
        } else {
            comboBox->setCurrentIndex(i + 2);
        }
    }
    isGroupComboBoxReady = true;
}

void SolutionsForm::selectGroupComboVariant(int index)
{
    if (index == 0) {
        updateComboBox(ui->userNameComboBox, getUserNames());
    } else if (index == 1) {
        updateComboBox(ui->userNameComboBox, getUserNamesWithoutGroup());
    } else {
        auto id = ui->groupNameComboBox->itemData(index).toUuid();
        const auto& group = getGroup(id);
        updateComboBox(ui->userNameComboBox, group.sortedUserNames);
    }
}

void SolutionsForm::updateButtons()
{
    const auto& rows = ui->tableWidget->selectionModel()->selectedRows();
    bool isRowSelected = rows.size() == 1;
    ui->selectSectionButton->setEnabled(isRowSelected);
    ui->selectUserButton->setEnabled(isRowSelected);
    ui->selectGroupButton->setEnabled(
                isRowSelected
                && !ui->tableWidget->item(rows[0].row(), 3)->data(Qt::UserRole).isNull());
    ui->openButton->setEnabled(isRowSelected);
}

void SolutionsForm::openSolutionInRow(int row)
{
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

void SolutionsForm::on_selectGroupButton_clicked()
{
    int row = ui->tableWidget->selectionModel()->selectedRows()[0].row();
    auto idVariant = ui->tableWidget->item(row, 3)->data(Qt::UserRole);
    if (idVariant.isNull())
        return;
    auto id = idVariant.toUuid();
    for (int i = 2; i < ui->groupNameComboBox->count(); ++i) {
        if (ui->groupNameComboBox->itemData(i).toUuid() == id) {
            ui->groupNameComboBox->setCurrentIndex(i);
            applyFilter();
            return;
        }
    }
}

void SolutionsForm::on_groupNameComboBox_currentIndexChanged(int index)
{
    if (!isGroupComboBoxReady)
        return;
    selectGroupComboVariant(index);
}
