#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include <omkit/section.h>

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

    void setSection(const Section& section);

public slots:
    void updateProgress();

signals:
    void requestedOpen(Section);

private slots:
    void on_enterButton_clicked();

private:
    Ui::SectionWidget *ui;

    Section section;
};

#endif // SECTIONWIDGET_H
