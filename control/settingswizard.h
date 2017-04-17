#ifndef SETTINGSWIZARD_H
#define SETTINGSWIZARD_H

#include <QDialog>

namespace Ui {
class SettingsWizard;
}

class SettingsWizard : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWizard(QWidget *parent = 0);
    ~SettingsWizard();

    void restart();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    void goTo(QWidget* nextPage);
    void updateButtons();
    bool createCommonDirectory(QString path);
    bool applySettings();

private slots:
    void on_cancelButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_choosePathButton_clicked();

private:
    Ui::SettingsWizard *ui;
    QList<QWidget*> prevPages;
};

#endif // SETTINGSWIZARD_H
