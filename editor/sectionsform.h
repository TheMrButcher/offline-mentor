#ifndef SECTIONSFORM_H
#define SECTIONSFORM_H

#include "section.h"

#include <QWidget>
#include <QList>

namespace Ui {
class SectionsForm;
}

class SectionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionsForm(QWidget *parent = 0);
    ~SectionsForm();

public slots:
    void setSections(QList<Section> sections);

private:
    Ui::SectionsForm *ui;
};

#endif // SECTIONSFORM_H
