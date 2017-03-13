#ifndef CREATESECTIONDIALOG_H
#define CREATESECTIONDIALOG_H

#include "section.h"

#include <QDialog>

namespace Ui {
class CreateSectionDialog;
}

class CreateSectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSectionDialog(QWidget *parent = 0);
    ~CreateSectionDialog();

    Section result() const;

public slots:
    void accept();

private slots:
    void on_choosePathButton_clicked();

private:
    Ui::CreateSectionDialog *ui;

    Section section;
};

#endif // CREATESECTIONDIALOG_H
