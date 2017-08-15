#include "textexplorer.h"
#include "ui_textexplorer.h"
#include <omkit/html_utils.h>
#include <omkit/caseimage.h>

TextExplorer::TextExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextExplorer)
{
    ui->setupUi(this);
}

TextExplorer::~TextExplorer()
{
    delete ui;
}

void TextExplorer::setTitle(QString title)
{
    ui->titleLabel->setText(title);
}

void TextExplorer::setPlainText(QString text)
{
    ui->textBrowser->setPlainText(text);
}

bool TextExplorer::load(QString path)
{
    QString html = readHTML(path);
    if (html.isEmpty())
        return false;
    ui->textBrowser->setHtml(html);
    return true;
}

bool TextExplorer::load(QDir dir, QString fileName, const CaseImage& image)
{
    QString html = readHTML(dir.absoluteFilePath(fileName));
    if (html.isEmpty())
        return false;
    setImageAndHTML(dir, image, html, ui->textBrowser);
    return true;

}
