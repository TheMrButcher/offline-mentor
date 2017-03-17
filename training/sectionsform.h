#ifndef SECTIONSFORM_H
#define SECTIONSFORM_H

#include <omkit/section.h>

#include <QList>
#include <QWidget>

namespace Ui {
class SectionsForm;
}

class SectionWidget;

class SectionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionsForm(QWidget *parent = 0);
    ~SectionsForm();

    void setUserName(QString name);

signals:
    void requestedOpen(Section);

public slots:
    void setSections(QList<Section> sections);
    void updateProgress();

private:
    Ui::SectionsForm *ui;
    QList<SectionWidget*> sectionWidgets;
};

#endif // SECTIONSFORM_H
