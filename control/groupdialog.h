#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <omkit/group.h>
#include <QDialog>

namespace Ui {
class GroupDialog;
}

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = 0);
    ~GroupDialog();

    void init(const Group& group);
    Group result();

    // QDialog interface
public slots:
    virtual void accept() override;

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_listWidget_itemSelectionChanged();
    void on_userNameEdit_textChanged(const QString &newText);

private:
    Ui::GroupDialog *ui;
    QUuid id;
};

#endif // GROUPDIALOG_H
