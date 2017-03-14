#include "sectioneditform.h"
#include "ui_sectioneditform.h"

SectionEditForm::SectionEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionEditForm)
{
    ui->setupUi(this);
    rootItem = ui->treeWidget->topLevelItem(0);
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
    this->originalSection = section;

    for (int i = 0; i < rootItem->childCount(); ++i) {
        auto caseRootItem = rootItem->child(i);
        nodes[caseRootItem].data->disconnect();
        rootItem->removeChild(caseRootItem);
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
        section.cases.append(nodes[caseRootItem].data->value);
    }

    return section;
}

void SectionEditForm::on_addCaseButton_clicked()
{
    Case caseValue;
    caseValue.name = "Новый кейс";
    addCase(caseValue);
}

void SectionEditForm::on_treeWidget_currentItemChanged(
        QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    if (nodes.contains(previous)) {
        nodes[previous].data->disconnect();
    }

    if (current == rootItem) {
        select(ui->sectionPage);
    } else {
        const auto& node = nodes[current];
        auto& caseValue = node.data->value;
        ui->caseNameEdit->setText(caseValue.name);
        auto caseRootItem = node.data->rootItem;
        node.data->connections.append(
                    connect(ui->caseNameEdit, &QLineEdit::textEdited,
                    [&caseValue, caseRootItem](QString name) {
                        caseValue.name = name;
                        caseRootItem->setText(0, name);
                    }));
        select(ui->casePage);
    }
}

void SectionEditForm::addCase(const Case& caseValue)
{
    QTreeWidgetItem* caseRootItem = new QTreeWidgetItem();
    caseRootItem->setIcon(0, QIcon(":/icons/case.png"));
    caseRootItem->setText(0, caseValue.name);

    QSharedPointer<CaseData> caseData(new CaseData);
    caseData->value = caseValue;

    caseData->items.append(caseRootItem);
    caseData->rootItem = caseRootItem;
    rootItem->addChild(caseRootItem);

    nodes[caseRootItem].set(NodeType::CaseRoot, caseData);
}

void SectionEditForm::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}

void SectionEditForm::CaseData::disconnect()
{
    foreach (const auto& connection, connections) {
        QObject::disconnect(connection);
    }
    connections.clear();
}
