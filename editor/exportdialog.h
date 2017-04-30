#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

    void initUI();

    // QDialog interface
public slots:
    virtual void accept() override;

private slots:
    void on_choosePathButton_clicked();

    void on_allCheckBox_clicked();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
