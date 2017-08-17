#ifndef SOLUTIONSFORM_H
#define SOLUTIONSFORM_H

#include <QWidget>

namespace Ui {
class SolutionsForm;
}

class QComboBox;
class Solution;
class QItemSelection;

class SolutionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SolutionsForm(QWidget *parent = 0);
    ~SolutionsForm();

signals:
    void requestedOpen(const Solution& solution);

public slots:
    void reload();
    void onGroupCollectionChanged();

private slots:
    void applyFilter();
    void onSelectionChanged(const QItemSelection &, const QItemSelection &);
    void on_resetFilterButton_clicked();
    void on_selectSectionButton_clicked();
    void on_selectUserButton_clicked();
    void on_openButton_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_selectGroupButton_clicked();
    void on_groupNameComboBox_currentIndexChanged(int index);

private:
    void fillTable(const QList<Solution>& solutions);
    void updateComboBox(QComboBox* comboBox, const QStringList& variants);
    void updateGroupComboBox();
    void selectGroupComboVariant(int index);
    void updateButtons();
    void openSolutionInRow(int row);

    Ui::SolutionsForm *ui;
    bool isGroupComboBoxReady = true;
};

#endif // SOLUTIONSFORM_H
