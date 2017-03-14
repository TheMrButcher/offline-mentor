#include "texteditorpage.h"
#include "ui_texteditorpage.h"

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
