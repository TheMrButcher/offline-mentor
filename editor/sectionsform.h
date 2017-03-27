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
    void requestedCreation();
    void requestedOpen(Section);

public slots:
    void load();
    void addSection(const Section& section);
    void updateSection(const Section& section);

private slots:
    void on_createSectionButton_clicked();

private:
    void updateView();

    Ui::SectionsForm *ui;

    QHash<QUuid, SectionWidget*> widgets;
};

#endif // SECTIONSFORM_H
