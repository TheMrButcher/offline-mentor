#include "texteditorpage.h"
#include "ui_texteditorpage.h"
#include "richtextedit.h"
#include <omkit/html_utils.h>

TextEditorPage::TextEditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditorPage)
{
    ui->setupUi(this);

    textEdit = new RichTextEdit(this);
    ui->verticalLayout->addWidget(textEdit);
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
    return writeHTML(path, textEdit->document());
}

bool TextEditorPage::load()
{
    QString path = dir.absoluteFilePath(myFileName);
    QString html = readHTML(path);
    if (html.isEmpty())
        return false;
    textEdit->setHtml(html);
    return true;
}
