#ifndef SOLUTIONEXPLORER_H
#define SOLUTIONEXPLORER_H

#include <QWidget>
#include <QHash>
#include <QUuid>

namespace Ui {
class SolutionExplorer;
}

class Solution;
class QListWidgetItem;

class SolutionExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit SolutionExplorer(QWidget *parent = 0);
    ~SolutionExplorer();

    void setSolution(const Solution& solution);

signals:
    void authorRenamed();

private slots:
    void selectCase(int caseIndex);

    void on_listWidget_itemSelectionChanged();
    void on_editUserNameButton_clicked();

private:
    Ui::SolutionExplorer *ui;

    struct Descriptor {
        int pageIndex;
        QListWidgetItem* item;
    };

    QHash<int, Descriptor> caseDescriptors;
    QUuid sectionId;
    QString userName;
};

#endif // SOLUTIONEXPLORER_H
