#ifndef TRAININGCREATIONWIZARD_H
#define TRAININGCREATIONWIZARD_H

#include <QDialog>

namespace Ui {
class TrainingCreationWizard;
}

class TrainingCreationWizard : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingCreationWizard(QWidget *parent = 0);
    ~TrainingCreationWizard();

    void restart();

public slots:
    void onGroupAdded(const QUuid& id);

signals:
    void groupCreationRequested();

private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_choosePathButton_clicked();
    void on_selectGroupsButton_toggled(bool checked);
    void on_customLoginsBox_toggled(bool checked);

private:
    void goTo(QWidget* nextPage);
    void updateButtons();
    bool saveDirectory(QString path);

    Ui::TrainingCreationWizard *ui;
    QList<QWidget*> prevPages;
};

#endif // TRAININGCREATIONWIZARD_H
