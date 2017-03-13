#ifndef SECTIONSFORM_H
#define SECTIONSFORM_H

#include "section.h"

#include <QWidget>
#include <QList>

namespace Ui {
class SectionsForm;
}

class CreateSectionDialog;

class SectionsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SectionsForm(QWidget *parent = 0);
    ~SectionsForm();

signals:
    void requestedOpen(Section);

public slots:
    void setSections(QList<Section> sections);

private slots:
    void onSectionCreated();
    void on_createSectionButton_clicked();

private:
    void updateView();

    Ui::SectionsForm *ui;

    CreateSectionDialog* createSectionDialog;
    QList<Section> sections;
};

#endif // SECTIONSFORM_H
