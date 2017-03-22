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

public slots:
    void reload();

private slots:
    void applyFilter();
    void onSelectionChanged(const QItemSelection &, const QItemSelection &);
    void on_resetFilterButton_clicked();

    void on_selectSectionButton_clicked();

    void on_selectUserButton_clicked();

private:
    void fillTable(const QList<Solution>& solutions);
    void updateComboBox(QComboBox* comboBox, const QStringList& variants);
    void updateButtons();

    Ui::SolutionsForm *ui;
};

#endif // SOLUTIONSFORM_H
