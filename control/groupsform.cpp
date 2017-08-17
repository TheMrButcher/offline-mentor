#include "groupsform.h"
#include "ui_groupsform.h"
#include "group_utils.h"
#include "groupdialog.h"

GroupsForm::GroupsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupsForm)
{
    ui->setupUi(this);
}

GroupsForm::~GroupsForm()
{
    delete ui;
}

void GroupsForm::load()
{
    for (const auto& group : getGroups()) {
        QListWidgetItem* item = new QListWidgetItem(group.name, ui->listWidget);
        item->setData(Qt::UserRole, group.id);
    }
}

void GroupsForm::editGroupInRow(int row)
{
    if (!groupDialog)
        groupDialog = new GroupDialog(this);
    auto item = ui->listWidget->item(row);
    const auto& group = getGroup(item->data(Qt::UserRole).toUuid());
    groupDialog->init(group);
    if (groupDialog->exec() == QDialog::Accepted) {
        auto newGroup = groupDialog->result();
        item->setText(newGroup.name);
        addGroup(newGroup);
        emit groupCollectionChanged();
    }
}

void GroupsForm::on_addButton_clicked()
{
    if (!groupDialog)
        groupDialog = new GroupDialog(this);
    groupDialog->init(Group::createGroup());
    if (groupDialog->exec() == QDialog::Accepted) {
        auto newGroup = groupDialog->result();
        QListWidgetItem* item = new QListWidgetItem(newGroup.name, ui->listWidget);
        item->setData(Qt::UserRole, newGroup.id);
        addGroup(newGroup);
        emit groupCollectionChanged();
    }
}

void GroupsForm::on_editButton_clicked()
{
    editGroupInRow(ui->listWidget->selectionModel()->selectedRows()[0].row());
}

void GroupsForm::on_removeButton_clicked()
{
    int row = ui->listWidget->selectionModel()->selectedRows()[0].row();
    auto item = ui->listWidget->item(row);
    removeGroup(item->data(Qt::UserRole).toUuid());
    delete item;
    ui->listWidget->selectionModel()->clear();
    emit groupCollectionChanged();
}

void GroupsForm::on_listWidget_doubleClicked(const QModelIndex &index)
{
    editGroupInRow(index.row());
}

void GroupsForm::on_listWidget_itemSelectionChanged()
{
    bool isRowSelected = ui->listWidget->selectionModel()->selectedRows().size() == 1;
    ui->editButton->setEnabled(isRowSelected);
    ui->removeButton->setEnabled(isRowSelected);
}
