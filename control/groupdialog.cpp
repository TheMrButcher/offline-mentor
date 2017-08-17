#include "groupdialog.h"
#include "ui_groupdialog.h"
#include "group_utils.h"
#include <QMessageBox>

GroupDialog::GroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
}

GroupDialog::~GroupDialog()
{
    delete ui;
}

void GroupDialog::init(const Group& group)
{
    id = group.id;
    ui->nameEdit->setText(group.name);

    ui->listWidget->clear();
    ui->listWidget->selectionModel()->clear();

    for (const auto& userName : group.sortedUserNames) {
        QListWidgetItem* item = new QListWidgetItem(userName, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

Group GroupDialog::result()
{
    Group group;
    group.id = id;
    group.name = ui->nameEdit->text();
    for (int row = 0; row < ui->listWidget->count(); ++row)
        group.userNames.insert(ui->listWidget->item(row)->text());
    return group;
}

void GroupDialog::accept()
{
    if (ui->nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Неверные данные", "Пожалуйста, укажите название группы");
        return;
    }
    QDialog::accept();
}

void GroupDialog::on_addButton_clicked()
{
    QListWidgetItem* item = new QListWidgetItem(
                ui->userNameEdit->text(), ui->listWidget);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void GroupDialog::on_removeButton_clicked()
{
    int row = ui->listWidget->selectionModel()->selectedRows()[0].row();
    delete ui->listWidget->item(row);
    ui->listWidget->selectionModel()->clear();
}

void GroupDialog::on_listWidget_itemSelectionChanged()
{
    bool isRowSelected = ui->listWidget->selectionModel()->selectedRows().size() == 1;
    ui->removeButton->setEnabled(isRowSelected);
}

void GroupDialog::on_userNameEdit_textChanged(const QString &newText)
{
    ui->addButton->setEnabled(!newText.isEmpty());
}
