#include "texteditorpage.h"
#include "ui_texteditorpage.h"
#include "richtextedit.h"
#include <omkit/html_utils.h>

TextEditorPage::TextEditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditorPage)
{
    ui->setupUi(this);

    myTextEdit = new RichTextEdit(this);
    ui->verticalLayout->addWidget(myTextEdit);
}

TextEditorPage::~TextEditorPage()
{
    delete ui;
}

void TextEditorPage::setTitle(QString title)
{
    ui->titleLabel->setText(title);
}

void TextEditorPage::setFilePath(QDir dir, QString fileName)
{
    this->dir = dir;
    myFileName = fileName;
}

QString TextEditorPage::fileName() const
{
    return myFileName;
}

bool TextEditorPage::save()
{
    if (myFileName.isEmpty())
        return false;
    QString path = dir.absoluteFilePath(myFileName);
    return writeHTML(path, myTextEdit->document());
}

bool TextEditorPage::load()
{
    QString path = dir.absoluteFilePath(myFileName);
    QString html = readHTML(path);
    if (html.isEmpty())
        return false;
    myTextEdit->setHtml(html);
    myTextEdit->document()->setModified(false);
    return true;
}

bool TextEditorPage::removeFile()
{
    if (!dir.exists(myFileName))
        return true;
    return dir.remove(myFileName);
}

RichTextEdit* TextEditorPage::textEdit()
{
    return myTextEdit;
}
