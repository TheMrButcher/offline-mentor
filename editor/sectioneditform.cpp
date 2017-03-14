#include "sectioneditform.h"
#include "casepage.h"
#include "texteditorpage.h"
#include "ui_sectioneditform.h"

SectionEditForm::SectionEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionEditForm)
{
    ui->setupUi(this);
    rootItem = ui->treeWidget->topLevelItem(0);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);
}

SectionEditForm::~SectionEditForm()
{
    delete ui;
}

Section SectionEditForm::section() const
{
    Section result = sectionFromUI();
    result.path = originalSection.path;
    return result;
}

void SectionEditForm::setSection(const Section& section)
{
    select(ui->sectionPage);
    this->originalSection = section;
    setSectionName(section.name);

    for (int i = 0; i < rootItem->childCount(); ++i) {
        auto caseRootItem = rootItem->child(i);
        auto pages = nodes[caseRootItem].pages;
        delete pages.mainPage;
        delete pages.questionPage;
        delete caseRootItem;
    }
    nodes.clear();

    ui->nameEdit->setText(section.name);
    ui->descriptionEdit->setPlainText(section.description);

    foreach (const auto& caseValue, section.cases)
        addCase(caseValue);
}

Section SectionEditForm::sectionFromUI() const
{
    Section section;
    section.name = ui->nameEdit->text();
    section.description = ui->descriptionEdit->toPlainText();

    for (int i = 0; i < rootItem->childCount(); ++i) {
        auto caseRootItem = rootItem->child(i);
        auto pages = nodes[caseRootItem].pages;
        Case caseValue = pages.mainPage->getCase();
        section.cases.append(caseValue);
    }

    return section;
}

void SectionEditForm::on_addCaseButton_clicked()
{
    Case caseValue;
    caseValue.name = "Новый";
    addCase(caseValue);
}

void SectionEditForm::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem*)
{
    if (current == rootItem) {
        select(ui->sectionPage);
    } else {
        ui->stackedWidget->setCurrentIndex(nodes[current].pageId);
    }
}

void SectionEditForm::on_nameEdit_textEdited(const QString &arg)
{
    setSectionName(arg);
}

void SectionEditForm::setSectionName(QString name)
{
    rootItem->setText(0, "Раздел \"" + name + "\"");
}

void SectionEditForm::addCase(const Case& caseValue)
{
    QTreeWidgetItem* caseRootItem = new QTreeWidgetItem();
    caseRootItem->setIcon(0, QIcon(":/icons/case.png"));
    caseRootItem->setText(0, "Кейс \"" + caseValue.name + "\"");
    rootItem->addChild(caseRootItem);

    CasePage* casePage = new CasePage;
    int casePageId = ui->stackedWidget->addWidget(casePage);
    casePage->setCase(caseValue);
    casePage->connectWith(caseRootItem);

    QTreeWidgetItem* questionItem = new QTreeWidgetItem();
    questionItem->setIcon(0, QIcon(":/icons/question.png"));
    questionItem->setText(0, "Вопрос");
    caseRootItem->addChild(questionItem);

    TextEditorPage* questionPage = new TextEditorPage;
    questionPage->setTitle("Текст вопроса");
    int questionPageId = ui->stackedWidget->addWidget(questionPage);

    CasePages pages{ casePage, questionPage };

    nodes[caseRootItem] = NodeDescriptor{ casePageId, pages };
    nodes[questionItem] = NodeDescriptor{ questionPageId, pages };
}

void SectionEditForm::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
