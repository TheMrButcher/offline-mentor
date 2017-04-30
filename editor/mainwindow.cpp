#include "mainwindow.h"
#include "sectionsform.h"
#include "sectioneditform.h"
#include "createsectiondialog.h"
#include "aboutdialog.h"
#include "settings.h"
#include "section_utils.h"
#include "richtextedit.h"
#include "exportdialog.h"
#include "ui_mainwindow.h"

#include <omkit/utils.h>
#include <omkit/string_utils.h>
#include <omkit/omkit.h>

#include <QFontComboBox>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QClipboard>
#include <QMimeData>
#include <QTextList>

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
    fontSizeComboBox->setCurrentText("14");
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
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(updatePasteButton()));
    connect(ui->cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->alignLeftAction, SIGNAL(triggered(bool)), this, SLOT(align(bool)));
    connect(ui->alignCenterAction, SIGNAL(triggered(bool)), this, SLOT(align(bool)));
    connect(ui->alignRightAction, SIGNAL(triggered(bool)), this, SLOT(align(bool)));
    connect(ui->alignJustifyAction, SIGNAL(triggered(bool)), this, SLOT(align(bool)));
    connect(ui->orderedListAction, SIGNAL(triggered(bool)), this, SLOT(list(bool)));
    connect(ui->unorderedListAction, SIGNAL(triggered(bool)), this, SLOT(list(bool)));
    connect(ui->undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    connect(ui->addCaseAction, SIGNAL(triggered()), this, SLOT(addCase()));
    connect(ui->removeCaseAction, SIGNAL(triggered()), this, SLOT(removeCase()));
    connect(ui->exportSectionsAction, SIGNAL(triggered()), this, SLOT(showExportDialog()));

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

void MainWindow::cut()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        textEdit->setFocus();
        textEdit->cut();
    }
}

void MainWindow::copy()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        textEdit->setFocus();
        textEdit->copy();
    }
}

void MainWindow::paste()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        textEdit->setFocus();
        textEdit->paste();
    }
}

void MainWindow::align(bool toggled)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QObject* sender = QObject::sender();
        Qt::Alignment alignment;
        if (toggled) {
            if (sender == ui->alignLeftAction) {
                alignment = Qt::AlignLeft | Qt::AlignAbsolute;
            } else if (sender == ui->alignCenterAction) {
                alignment = Qt::AlignHCenter;
            } else if (sender == ui->alignRightAction) {
                alignment = Qt::AlignRight | Qt::AlignAbsolute;
            } else if (sender == ui->alignJustifyAction) {
                alignment = Qt::AlignJustify;
            } else {
                return;
            }
        } else {
            if (sender == ui->alignLeftAction
                || sender == ui->alignCenterAction
                || sender == ui->alignRightAction) {
                alignment = Qt::AlignJustify;
            } else if (sender == ui->alignJustifyAction) {
                alignment = Qt::AlignLeft | Qt::AlignAbsolute;
            } else {
                return;
            }
        }
        textEdit->setAlignment(alignment);
        updateAlignmentButtons(alignment);
        textEdit->setFocus();
    }
}

void MainWindow::list(bool toggled)
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QObject* sender = QObject::sender();
        if (toggled) {
            if (sender == ui->orderedListAction) {
                textEdit->applyListStyle(QTextListFormat::ListDecimal);
            } else if (sender == ui->unorderedListAction) {
                textEdit->applyListStyle(QTextListFormat::ListDisc);
            } else {
                return;
            }
        } else {
             if (sender == ui->orderedListAction
                 || sender == ui->unorderedListAction) {
                 textEdit->clearListFormat();
             } else {
                 return;
             }
        }
        updateListButtons();
        textEdit->setFocus();
    }
}

void MainWindow::undo()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        textEdit->undo();
        textEdit->setFocus();
        updateHistoryButtons();
    }
}

void MainWindow::redo()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        textEdit->redo();
        textEdit->setFocus();
        updateHistoryButtons();
    }
}

void MainWindow::addCase()
{
    if (SectionEditForm* sectionEditForm = currentSectionEditForm())
        sectionEditForm->addCase();
}

void MainWindow::removeCase()
{
    if (SectionEditForm* sectionEditForm = currentSectionEditForm())
        sectionEditForm->removeCurrentCase();
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
    connect(sectionEditForm, SIGNAL(caseInFocus(bool)),
            this, SLOT(onCaseInFocus(bool)));
    connect(sectionEditForm, SIGNAL(textEditInFocus(bool)),
            this, SLOT(onTextEditInFocus(bool)));
    connect(sectionEditForm, SIGNAL(fontChanged(QFont)),
            this, SLOT(onFontChanged(QFont)));
    connect(sectionEditForm, SIGNAL(selectionChanged()),
            this, SLOT(onSelectionChanged()));
    connect(sectionEditForm, SIGNAL(cursorPositionChanged()),
            this, SLOT(onCursorPositionChanged()));
    connect(sectionEditForm, SIGNAL(undoAvailable(bool)),
            this, SLOT(onHistoryAvailable(bool)));
    connect(sectionEditForm, SIGNAL(redoAvailable(bool)),
            this, SLOT(onHistoryAvailable(bool)));
    connect(sectionEditForm, SIGNAL(modificationChanged(bool)),
            this, SLOT(onModificationChanged(bool)));
}

void MainWindow::showExportDialog()
{
    if (!exportDialog)
        exportDialog = new ExportDialog(this);
    exportDialog->initUI();
    exportDialog->exec();
}

void MainWindow::onSectionSaved(const Section& section)
{
    auto widget = openedPages[section.id];
    int index = ui->tabWidget->indexOf(widget);
    ui->tabWidget->setTabText(index, trim(section.name, 16));
    sectionsForm->updateSection(section);
}

void MainWindow::onCaseInFocus(bool inFocus)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    setCaseEditButtonsEnabled(inFocus);
}

void MainWindow::onTextEditInFocus(bool inFocus)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    setTextEditButtonsEnabled(inFocus);
}

void MainWindow::onFontChanged(const QFont& font)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    updateFontButtons(font);
}

void MainWindow::onSelectionChanged()
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    RichTextEdit* textEdit = currentTextEdit();
    setCopyAndCutButtonsEnabled(
                textEdit && textEdit->textCursor().hasSelection());
}

void MainWindow::updatePasteButton()
{
    if (!currentTextEdit()) {
        ui->pasteAction->setEnabled(false);
        return;
    }

    const QMimeData* mimeData = QApplication::clipboard()->mimeData();
    ui->pasteAction->setEnabled(mimeData->hasText() || mimeData->hasHtml());
}

void MainWindow::updateHistoryButtons()
{
    if (RichTextEdit* textEdit = currentTextEdit()) {
        QTextDocument* document = textEdit->document();
        ui->undoAction->setEnabled(document->isUndoAvailable());
        ui->redoAction->setEnabled(document->isRedoAvailable());
    } else {
        ui->undoAction->setEnabled(false);
        ui->redoAction->setEnabled(false);
    }
}

void MainWindow::onCursorPositionChanged()
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    if (RichTextEdit* textEdit = currentTextEdit()) {
        updateAlignmentButtons(textEdit->alignment());
        updateListButtons();
    }
}

void MainWindow::onHistoryAvailable(bool /*ignored*/)
{
    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    updateHistoryButtons();
}

void MainWindow::onModificationChanged(bool changed)
{
    SectionEditForm* sectionEditForm = (SectionEditForm*)QObject::sender();
    int index = ui->tabWidget->indexOf(sectionEditForm);
    if (index == -1)
        return;
    QString title = trim(sectionEditForm->sectionName(), 16)
            + (changed ? QString("*") : "");
    ui->tabWidget->setTabText(index, title);

    if (QObject::sender() != ui->tabWidget->currentWidget())
        return;
    ui->saveAction->setEnabled(changed);
}

bool MainWindow::isSectionsFormCurrent() const
{
    return ui->tabWidget->currentWidget() == sectionsForm;
}

SectionEditForm* MainWindow::currentSectionEditForm()
{
    if (isSectionsFormCurrent())
        return nullptr;
    return (SectionEditForm*)ui->tabWidget->currentWidget();
}

RichTextEdit* MainWindow::currentTextEdit()
{
    if (SectionEditForm* sectionEditForm = currentSectionEditForm())
        return sectionEditForm->currentTextEdit();
    return nullptr;
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
    ui->alignLeftAction->setEnabled(enabled);
    ui->alignCenterAction->setEnabled(enabled);
    ui->alignRightAction->setEnabled(enabled);
    ui->alignJustifyAction->setEnabled(enabled);
    ui->orderedListAction->setEnabled(enabled);
    ui->unorderedListAction->setEnabled(enabled);

    if (enabled) {
        RichTextEdit* textEdit = currentTextEdit();
        QTextCursor cursor = textEdit->textCursor();

        QTextCharFormat format = cursor.charFormat();
        updateFontButtons(format.font());

        setCopyAndCutButtonsEnabled(cursor.hasSelection());
        updateAlignmentButtons(textEdit->alignment());
        updateListButtons();
    } else {
        setCopyAndCutButtonsEnabled(false);
    }
    updatePasteButton();
    updateHistoryButtons();
}

void MainWindow::setCaseEditButtonsEnabled(bool enabled)
{
    ui->removeCaseAction->setEnabled(enabled);
}

void MainWindow::updateFontButtons(const QFont& font)
{
    ui->boldAction->setChecked(font.bold());
    ui->italicAction->setChecked(font.italic());
    ui->underlineAction->setChecked(font.underline());
    fontComboBox->setCurrentFont(font);
    fontSizeComboBox->setCurrentText(QString::number(font.pointSize()));
}

void MainWindow::setCopyAndCutButtonsEnabled(bool hasSelectedText)
{
    ui->cutAction->setEnabled(hasSelectedText);
    ui->copyAction->setEnabled(hasSelectedText);
}

void MainWindow::updateAlignmentButtons(Qt::Alignment alignment)
{
    ui->alignLeftAction->setChecked(alignment & Qt::AlignLeft);
    ui->alignCenterAction->setChecked(alignment & Qt::AlignHCenter);
    ui->alignRightAction->setChecked(alignment & Qt::AlignRight);
    ui->alignJustifyAction->setChecked(alignment & Qt::AlignJustify);
}

void MainWindow::updateListButtons()
{
    QTextList* textList = currentTextEdit()->textCursor().currentList();
    if (textList) {
        QTextListFormat::Style style = textList->format().style();
        ui->orderedListAction->setChecked(style == QTextListFormat::ListDecimal);
        ui->unorderedListAction->setChecked(style == QTextListFormat::ListDisc);
    } else {
        ui->orderedListAction->setChecked(false);
        ui->unorderedListAction->setChecked(false);
    }
}

bool MainWindow::closePage(SectionEditForm* sectionEditForm)
{
    if (sectionEditForm->hasChanges()) {
        int answer = QMessageBox::question(
                    this, "Несохраненные изменения",
                    "Раздел \"" + sectionEditForm->sectionName() + "\" "
                    "был изменен. Сохранить изменения?",
                    QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (answer == QMessageBox::Cancel)
            return false;
        if (answer == QMessageBox::Yes)
            sectionEditForm->save();
    }
    auto id = sectionEditForm->sectionId();
    openedPages.remove(id);
    delete sectionEditForm;
    return true;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index == ui->tabWidget->indexOf(sectionsForm))
        return;
    SectionEditForm* sectionEditForm = currentSectionEditForm();
    closePage(sectionEditForm);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    bool isSectionEditForm = index != ui->tabWidget->indexOf(sectionsForm);
    ui->addCaseAction->setEnabled(isSectionEditForm);
    if (isSectionEditForm) {
        SectionEditForm* sectionEditForm = currentSectionEditForm();
        ui->saveAction->setEnabled(sectionEditForm->hasChanges());
        setCaseEditButtonsEnabled(sectionEditForm->isCaseInFocus());
        setTextEditButtonsEnabled(sectionEditForm->isTextEditInFocus());
        if (sectionEditForm->isTextEditInFocus())
            sectionEditForm->currentTextEdit()->setFocus();
    } else {
        ui->saveAction->setEnabled(false);
        setCaseEditButtonsEnabled(false);
        setTextEditButtonsEnabled(false);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    int sectionsFormIndex = ui->tabWidget->indexOf(sectionsForm);
    for (int i = ui->tabWidget->count() - 1; i >= 0; --i) {
        if (i == sectionsFormIndex)
            continue;
        auto sectionEditForm = (SectionEditForm*)ui->tabWidget->widget(i);
        if (!closePage(sectionEditForm)) {
            event->ignore();
            return;
        }
    }
    event->accept();
}
