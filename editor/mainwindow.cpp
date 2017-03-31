#include "mainwindow.h"
#include "sectionsform.h"
#include "sectioneditform.h"
#include "createsectiondialog.h"
#include "aboutdialog.h"
#include "settings.h"
#include "section_utils.h"
#include "richtextedit.h"
#include "ui_mainwindow.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/omkit.h>

#include <QFontComboBox>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Offline-наставник. Редактор v." + getVersion());
    showMaximized();

    OMKit::instance().init();

    fontComboBox = new QFontComboBox(this);
    fontComboBox->setCurrentFont(QFont("Times New Roman"));
    fontComboBox->setEnabled(false);
    ui->toolBar->insertWidget(ui->boldAction, fontComboBox);

    fontSizeComboBox = new QComboBox(this);
    foreach (int fontSize, QFontDatabase::standardSizes())
        fontSizeComboBox->addItem(QString::number(fontSize));
    fontSizeComboBox->setCurrentText("12");
    fontSizeComboBox->setEnabled(false);
    ui->toolBar->insertWidget(ui->boldAction, fontSizeComboBox);

    sectionsForm = new SectionsForm(this);
    int tabIndex = ui->tabWidget->addTab(sectionsForm, "Список разделов");
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::LeftSide, nullptr);
    ui->tabWidget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, nullptr);
    ui->tabWidget->setCurrentWidget(ui->tabWidget);

    connect(sectionsForm, SIGNAL(requestedCreation()), this, SLOT(create()));
    connect(sectionsForm, SIGNAL(requestedOpen(Section)), this, SLOT(openSection(Section)));

    connect(ui->createAction, SIGNAL(triggered()), this, SLOT(create()));
    connect(ui->openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
    connect(ui->clearFormatAction, SIGNAL(triggered()), this, SLOT(clearFormat()));
    connect(ui->boldAction, SIGNAL(triggered(bool)), this, SLOT(setBold(bool)));
    connect(ui->italicAction, SIGNAL(triggered(bool)), this, SLOT(setItalic(bool)));
    connect(ui->underlineAction, SIGNAL(triggered(bool)), this, SLOT(setUnderline(bool)));
    connect(fontComboBox, SIGNAL(activated(QString)), this, SLOT(setFontFamily(QString)));
    connect(fontSizeComboBox, SIGNAL(activated(QString)), this, SLOT(setFontSize(QString)));

    QTimer::singleShot(0, this, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create()
{
    if (!createSectionDialog) {
        createSectionDialog = new CreateSectionDialog(this);
    }
    int result = createSectionDialog->exec();
    if (result == QDialog::Accepted) {
        auto section = createSectionDialog->result();
        Settings::instance().updateLastDirectoryPath(section.path);
        sectionsForm->addSection(section);
        openSection(section);
    }
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Открытие раздела", Settings::instance().safeLastDirectoryPath(),
                "Файл раздела (*.oms)");
    if (path.isEmpty())
        return;

    Section section;
    if (isKnownSection(path)) {
        section = getSection(path);
    } else {
        section.path = path;
        if (!section.open()) {
            QMessageBox::warning(this, "Не удалось открыть раздел",
                                 "При открытии раздела произошла ошибка. Проверьте правильность "
                                 "указанного пути.");
            return;
        }
        sectionsForm->addSection(section);
    }
    Settings::instance().updateLastDirectoryPath(path);
    openSection(section);
}

void MainWindow::save()
{
    if (isSectionsFormCurrent())
        return;
    ((SectionEditForm*)ui->tabWidget->currentWidget())->save();
}

void MainWindow::selectAll()
{
    if (RichTextEdit* textEdit = currentTextEdit())
        textEdit->selectAll();
}

void MainWindow::clearFormat()
{
    if (RichTextEdit* textEdit = currentTextEdit())
        textEdit->clearFormat();
}

void MainWindow::setBold(bool bold)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextCharFormat format;
        format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
        textEdit->applyFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::setItalic(bool italic)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextCharFormat format;
        format.setFontItalic(italic);
        textEdit->applyFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::setUnderline(bool underline)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextCharFormat format;
        format.setFontUnderline(underline);
        textEdit->applyFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::setFontFamily(QString fontFamily)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextCharFormat format;
        format.setFontFamily(fontFamily);
        textEdit->applyFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::setFontSize(QString fontSizeStr)
{
    bool isNumber = false;
    int fontSize = fontSizeStr.toInt(&isNumber);
    if (!isNumber || fontSize < 6) {
        QMessageBox::warning(this, "Неверные данные", "Введите корректный размер шрифта.");
        return;
    }

    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextCharFormat format;
        format.setFontPointSize(fontSize);
        textEdit->applyFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::showAbout()
{
    if (!aboutDialog)
        aboutDialog = new AboutDialog(this);
    aboutDialog->exec();
}

void MainWindow::loadSettings()
{
    auto& settings = Settings::instance();
    settings.read();
    sectionsForm->load();
}

void MainWindow::openSection(const Section& section)
{
    if (openedPages.contains(section.id)) {
        ui->tabWidget->setCurrentWidget(openedPages[section.id]);
        return;
    }
    SectionEditForm* sectionEditForm = new SectionEditForm(this);
    sectionEditForm->setSection(section);
    ui->tabWidget->addTab(sectionEditForm, trim(section.name, 16));
    ui->tabWidget->setCurrentWidget(sectionEditForm);
    openedPages[section.id] = sectionEditForm;

    connect(sectionEditForm, SIGNAL(sectionSaved(Section)),
            this, SLOT(onSectionSaved(Section)));
    connect(sectionEditForm, SIGNAL(textEditInFocus(bool)),
            this, SLOT(onTextEditInFocus(bool)));
    connect(sectionEditForm, SIGNAL(fontChanged(QFont)),
            this, SLOT(onFontChanged(QFont)));
}

void MainWindow::onSectionSaved(const Section& section)
{
    auto widget = openedPages[section.id];
    int index = ui->tabWidget->indexOf(widget);
    ui->tabWidget->setTabText(index, trim(section.name, 16));
    sectionsForm->updateSection(section);
}

void MainWindow::onTextEditInFocus(bool inFocus)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    setTextEditButtonsEnabled(inFocus);
    if (inFocus)
        updateTextEditButtons();
}

void MainWindow::onFontChanged(const QFont& font)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    updateFontButtons(font);
}

bool MainWindow::isSectionsFormCurrent() const
{
    return ui->tabWidget->currentWidget() == sectionsForm;
}

RichTextEdit* MainWindow::currentTextEdit()
{
    if (isSectionsFormCurrent())
        return nullptr;
    SectionEditForm* sectionEditForm =
            (SectionEditForm*)ui->tabWidget->currentWidget();
    return sectionEditForm->currentTextEdit();
}

void MainWindow::setTextEditButtonsEnabled(bool enabled)
{
    ui->selectAllAction->setEnabled(enabled);
    ui->clearFormatAction->setEnabled(enabled);
    ui->boldAction->setEnabled(enabled);
    ui->italicAction->setEnabled(enabled);
    ui->underlineAction->setEnabled(enabled);
    fontComboBox->setEnabled(enabled);
    fontSizeComboBox->setEnabled(enabled);
}

void MainWindow::updateTextEditButtons()
{
    QTextCharFormat format = currentTextEdit()->textCursor().charFormat();
    updateFontButtons(format.font());
}

void MainWindow::updateFontButtons(const QFont& font)
{
    ui->boldAction->setChecked(font.bold());
    ui->italicAction->setChecked(font.italic());
    ui->underlineAction->setChecked(font.underline());
    fontComboBox->setCurrentFont(font);
    fontSizeComboBox->setCurrentText(QString::number(font.pointSize()));
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index == ui->tabWidget->indexOf(sectionsForm))
        return;
    auto widget = ui->tabWidget->widget(index);
    auto id = ((SectionEditForm*)widget)->sectionId();
    openedPages.remove(id);
    delete widget;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    bool isEditor = index != ui->tabWidget->indexOf(sectionsForm);
    ui->saveAction->setEnabled(isEditor);

    if (isEditor) {
        auto sectionEditForm = (SectionEditForm*)ui->tabWidget->widget(index);
        setTextEditButtonsEnabled(sectionEditForm->isTextEditInFocus());

        if (sectionEditForm->isTextEditInFocus()) {
            sectionEditForm->currentTextEdit()->setFocus();
            updateTextEditButtons();
        }
    } else {
        setTextEditButtonsEnabled(false);
    }
}
