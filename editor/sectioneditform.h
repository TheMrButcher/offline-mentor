#ifndef SECTIONEDITFORM_H
#define SECTIONEDITFORM_H

#include "section.h"

#include <QWidget>

namespace Ui {
class SectionEditForm;
}

class SectionEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionEditForm(QWidget *parent = 0);
    ~SectionEditForm();

    Section section() const;
    void setSection(const Section& section);

private:
    Section sectionFromUI() const;

    Ui::SectionEditForm *ui;

    Section originalSection;
};

#endif // SECTIONEDITFORM_H
