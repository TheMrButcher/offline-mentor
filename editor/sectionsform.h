#ifndef SECTIONSFORM_H
#define SECTIONSFORM_H

#include <omkit/section.h>

#include <QWidget>
#include <QList>

namespace Ui {
class SectionsForm;
}

class CreateSectionDialog;
class SectionWidget;

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
    void updateSection(const Section& section);

private slots:
    void onSectionCreated();
    void on_createSectionButton_clicked();

private:
    void updateView();

    Ui::SectionsForm *ui;

    CreateSectionDialog* createSectionDialog;
    QList<Section> sections;
    QHash<QUuid, SectionWidget*> widgets;
};

#endif // SECTIONSFORM_H
