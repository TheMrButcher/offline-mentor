#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include "section.h"

#include <QWidget>

namespace Ui {
class SectionWidget;
}

class SectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectionWidget(QWidget *parent = 0);
    ~SectionWidget();

    QString key() const;
    void setSection(const Section& section);

signals:
    void requestedOpen(Section);

private slots:
    void on_openButton_clicked();

private:
    Ui::SectionWidget *ui;
    Section section;
};

#endif // SECTIONWIDGET_H
