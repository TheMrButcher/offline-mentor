#ifndef SOLUTIONSFORM_H
#define SOLUTIONSFORM_H

#include <QWidget>

namespace Ui {
class SolutionsForm;
}

class SolutionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SolutionsForm(QWidget *parent = 0);
    ~SolutionsForm();

public slots:
    void reload();

private:
    Ui::SolutionsForm *ui;
};

#endif // SOLUTIONSFORM_H
