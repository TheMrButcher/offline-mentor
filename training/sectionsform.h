#ifndef SECTIONSFORM_H
#define SECTIONSFORM_H

#include <omkit/section.h>

#include <QList>
#include <QWidget>

namespace Ui {
class SectionsForm;
}

class SectionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionsForm(QWidget *parent = 0);
    ~SectionsForm();

    void setUserName(QString name);

public slots:
    void setSections(QList<Section> sections);

private:
    Ui::SectionsForm *ui;
};

#endif // SECTIONSFORM_H
