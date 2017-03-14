#include "texteditorpage.h"
#include "ui_texteditorpage.h"
#include <QTextDocumentWriter>

TextEditorPage::TextEditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditorPage)
{
    ui->setupUi(this);
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
    QTextDocumentWriter writer(path);
    return writer.write(ui->textEdit->document());
}
