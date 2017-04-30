#ifndef TOTALPAGE_H
#define TOTALPAGE_H

#include <QWidget>

namespace Ui {
class TotalPage;
}

class Section;

class TotalPage : public QWidget
{
    Q_OBJECT

public:
    explicit TotalPage(QWidget *parent = 0);
    ~TotalPage();

    void load(const Section& section);

    void setSuccess();
    void setTransferError();
    void setUnknownState();

signals:
    void requestedTransfer();
    void requestedArchiveSave(QString path);

private slots:
    void on_archiveButton_clicked();

private:
    QString sectionPath;
    Ui::TotalPage *ui;
};

#endif // TOTALPAGE_H
