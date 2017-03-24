#include "sectioneditform.h"
#include "casepage.h"
#include "texteditorpage.h"
#include "ui_sectioneditform.h"

#include <QMessageBox>

SectionEditForm::SectionEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectionEditForm)
{
    ui->setupUi(this);
    rootItem = ui->treeWidget->topLevelItem(0);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);
    ui->treeWidget->invisibleRootItem()->setFlags(
                ui->treeWidget->invisibleRootItem()->flags().setFlag(Qt::ItemIsDropEnabled, false));
}

SectionEditForm::~SectionEditForm()
{
    delete ui;
}

Section SectionEditForm::section() const
{
    Section result = sectionFromUI();
    result.copyHidden(originalSection);
    return result;
}

void SectionEditForm::setSection(const Section& section)
{
    ui->treeWidget->setCurrentItem(rootItem);
    select(ui->sectionPage);
    this->originalSection = section;
    setSectionName(originalSection.name);

    for (int i = rootItem->childCount() - 1; i >= 0; --i) {
        auto caseRootItem = rootItem->child(i);
        auto pages = nodes[caseRootItem].pages;
        delete pages.mainPage;
        delete pages.questionPage;
        delete pages.answerPage;
        delete caseRootItem;
    }
    nodes.clear();

    ui->titleLabel->setText("Раздел \"" + originalSection.name + "\"");
    ui->nameEdit->setText(originalSection.name);
    ui->descriptionEdit->setPlainText(originalSection.description);

    QStringList badFiles;
    for (int i = 0; i < originalSection.cases.size(); ++i) {
        auto& caseValue = originalSection.cases[i];
        if (caseValue.missingData()) {
            generateFileNames(caseValue);
            addCase(caseValue);
        } else {
            addCase(caseValue);
            auto caseRootItem = rootItem->child(i);
            auto pages = nodes[caseRootItem].pages;
            if (!pages.questionPage->load())
                badFiles.append(caseValue.name + "/Вопрос");
            if (!pages.answerPage->load())
                badFiles.append(caseValue.name + "/Ответ");
        }
    }

    if (!badFiles.isEmpty()) {
        QMessageBox::warning(this, "Ошибка при загрузке",
                             "Не удалось загрузить некоторые файлы кейсов: " + badFiles.join("; "));
        return;
    }
}

QUuid SectionEditForm::sectionId() const
{
    return originalSection.id;
}

void SectionEditForm::save()
{
    Section result = section();
    if (!result.save()) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось сохранить файл раздела: " + result.path);
        return;
    }

    QDir sectionDir = result.dir();
    QStringList badFiles;
    for (int i = 0; i < result.cases.size(); ++i) {
        auto pages = nodes[rootItem->child(i)].pages;
        const auto& caseValue = result.cases[i];

        if (!pages.questionPage->save())
            badFiles.append(caseValue.name + "/Вопрос");
        if (!pages.answerPage->save())
            badFiles.append(caseValue.name + "/Ответ");
    }

    if (!badFiles.isEmpty()) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось сохранить некоторые файлы кейсов: " + badFiles.join("; "));
        return;
    }
    emit sectionSaved(result);
}

Section SectionEditForm::sectionFromUI() const
{
    Section section;
    section.name = ui->nameEdit->text();
    section.description = ui->descriptionEdit->toPlainText();

    for (int i = 0; i < rootItem->childCount(); ++i) {
        auto caseRootItem = rootItem->child(i);
        auto pages = nodes[caseRootItem].pages;

        auto caseValue = pages.mainPage->getCase();
        caseValue.questionFileName = pages.questionPage->fileName();
        caseValue.answerFileName = pages.answerPage->fileName();
        section.cases.append(caseValue);
    }

    return section;
}

void SectionEditForm::on_addCaseButton_clicked()
{
    Case caseValue = Case::createCase();
    caseValue.name = "Новый";
    generateFileNames(caseValue);
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
    QString title = "Раздел \"" + name + "\"";
    ui->titleLabel->setText(title);
    rootItem->setText(0, title);
}

void SectionEditForm::addCase(const Case& caseValue)
{
    QTreeWidgetItem* caseRootItem = new QTreeWidgetItem();
    caseRootItem->setIcon(0, QIcon(":/icons/case.png"));
    caseRootItem->setText(0, "Кейс \"" + caseValue.name + "\"");
    caseRootItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
    rootItem->addChild(caseRootItem);

    CasePage* casePage = new CasePage;
    int casePageId = ui->stackedWidget->addWidget(casePage);
    casePage->setCase(caseValue);
    casePage->connectWith(caseRootItem);

    QDir sectionDir = originalSection.dir();

    QTreeWidgetItem* questionItem = new QTreeWidgetItem();
    questionItem->setIcon(0, QIcon(":/icons/question.png"));
    questionItem->setText(0, "Вопрос");
    questionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    caseRootItem->addChild(questionItem);

    TextEditorPage* questionPage = new TextEditorPage;
    questionPage->setTitle("Текст вопроса");
    questionPage->setFilePath(sectionDir, caseValue.questionFileName);
    int questionPageId = ui->stackedWidget->addWidget(questionPage);

    QTreeWidgetItem* answerItem = new QTreeWidgetItem();
    answerItem->setIcon(0, QIcon(":/icons/answer.png"));
    answerItem->setText(0, "Ответ наставника");
    answerItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    caseRootItem->addChild(answerItem);

    TextEditorPage* answerPage = new TextEditorPage;
    answerPage->setTitle("Текст ответа наставника");
    answerPage->setFilePath(sectionDir, caseValue.answerFileName);
    int answerPageId = ui->stackedWidget->addWidget(answerPage);

    CasePages pages{ casePage, questionPage, answerPage };

    nodes[caseRootItem] = NodeDescriptor{ casePageId, pages };
    nodes[questionItem] = NodeDescriptor{ questionPageId, pages };
    nodes[answerItem] = NodeDescriptor{ answerPageId, pages };
}

void SectionEditForm::generateFileNames(Case& c)
{
    QString caseFilePrefix = originalSection.nextCaseFilePrefix();
    if (caseFilePrefix.isEmpty())
        return;
    c.questionFileName = Case::makeQuestionFileName(caseFilePrefix);
    c.answerFileName = Case::makeAnswerFileName(caseFilePrefix);
}

void SectionEditForm::select(QWidget* widget)
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->indexOf(widget));
}
