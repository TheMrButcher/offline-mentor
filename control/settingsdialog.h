#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void initUI();

signals:
    void requestedSettingsWizard();
    void groupsPathChanged();

public slots:
    virtual void accept() override;

private slots:
    void on_chooseSectionsPathButton_clicked();
    void on_chooseSolutionsPathButton_clicked();
    void on_chooseGroupsPathButton_clicked();

private:
    Ui::SettingsDialog *ui;

    bool isInited = false;
};

#endif // SETTINGSDIALOG_H
