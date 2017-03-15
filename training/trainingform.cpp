#include "trainingform.h"
#include "questionpage.h"
#include "ui_trainingform.h"
#include <QMessageBox>

TrainingForm::TrainingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingForm)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);
}

TrainingForm::~TrainingForm()
{
    delete ui;
}

void TrainingForm::setSection(const Section& section)
{
    ui->listWidget->clear();
    for(int i = ui->stackedWidget->count() - 1; i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    nodes.clear();

    this->section = section;
    ui->titleLabel->setText("Раздел \"" + section.name + "\"");
    int nextCaseIndex = 1;
    QStringList badFiles;

    QListWidgetItem* itemToSelect = nullptr;
    int pageToShowId = 0;
    foreach (const auto& caseValue, section.cases) {
        QuestionPage* questionPage = new QuestionPage;
        if (!questionPage->loadCase(section, caseValue)) {
            delete questionPage;
            badFiles.append(caseValue.name + "/Вопрос");
            continue;
        }
        int questionPageId = ui->stackedWidget->addWidget(questionPage);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setText(QString("%1. Кейс \"%2\"").arg(nextCaseIndex).arg(caseValue.name));
        item->setIcon(QIcon(":/icons/question.png"));
        nodes[item] = NodeDescriptor{ questionPageId };

        if (!itemToSelect) {
            itemToSelect = item;
            pageToShowId = questionPageId;
        }

        ++nextCaseIndex;
    }

    if (itemToSelect) {
        ui->listWidget->setCurrentItem(itemToSelect);
        ui->stackedWidget->setCurrentIndex(pageToShowId);
    }

    if (!badFiles.isEmpty()) {
        QMessageBox::warning(this, "Ошибка при загрузке",
                             "Не удалось загрузить некоторые файлы кейсов: " + badFiles.join("; ")
                             + ". Они не будут показаны в списке кейсов.");
    }
}

void TrainingForm::on_listWidget_itemSelectionChanged()
{
    auto selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty())
        return;
    auto item = selectedItems.front();
    const auto& node = nodes[item];
    ui->stackedWidget->setCurrentIndex(node.questionPageId);
}
