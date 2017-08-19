#ifndef GROUPSFORM_H
#define GROUPSFORM_H

#include <QWidget>

namespace Ui {
class GroupsForm;
}

class GroupDialog;

class GroupsForm : public QWidget
{
    Q_OBJECT

public:
    explicit GroupsForm(QWidget *parent = 0);
    ~GroupsForm();

    void load();

public slots:
    void onGroupsPathChanged();
    void createGroup();

signals:
    void groupAdded(const QUuid& id);
    void groupCollectionChanged();

private:
    void editGroupInRow(int row);

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_removeButton_clicked();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_itemSelectionChanged();

private:
    Ui::GroupsForm *ui;
    GroupDialog* groupDialog = nullptr;
};

#endif // GROUPSFORM_H
