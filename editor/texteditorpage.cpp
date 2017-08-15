#include "texteditorpage.h"
#include "ui_texteditorpage.h"
#include "richtextedit.h"
#include <omkit/html_utils.h>
#include <QTextDocumentFragment>

namespace {
QString toString(CaseImage::HorAlign align)
{
    switch (align) {
    case CaseImage::HorAlign::Left: return "слева";
    case CaseImage::HorAlign::Center: return "по центру";
    case CaseImage::HorAlign::Right: return "справа";
    }
    return "";
}

QString toString(CaseImage::VertAlign align)
{
    switch (align) {
    case CaseImage::VertAlign::Top: return "над текстом";
    case CaseImage::VertAlign::Bottom: return "под текстом";
    }
    return "";
}
}

TextEditorPage::TextEditorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextEditorPage)
{
    ui->setupUi(this);

    myTextEdit = new RichTextEdit(this);
    ui->verticalLayout->addWidget(myTextEdit);

    resetImage();
    QPalette palette = ui->imageFrame->palette();
    palette.setColor(QPalette::Background, QColor(0xdd, 0xdd, 0xdd));
    ui->imageFrame->setPalette(palette);
    ui->imageFrame->setAutoFillBackground(true);
    connect(ui->openImageMenuButton, SIGNAL(clicked()), this, SIGNAL(requestedImageMenu()));
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

void TextEditorPage::setImage(const CaseImage& image)
{
    resetImage();
    if (image.isEmpty())
        return;

    ui->imageStateLabel->setText(QString(
                "К тексту привязано изображение: <b>%1</b><br>"
                "Размеры изображения: <b>%2 x %3</b> пикселей<br>"
                "Положение изображения: <b>%4 %5</b>")
                .arg(image.fileName).arg(image.width).arg(image.height)
                .arg(toString(image.vertAlign)).arg(toString(image.horAlign)));
    QPixmap pixmap(dir.absoluteFilePath(image.fileName));
    if (pixmap.isNull())
        return;
    ui->imagePreview->setPixmap(pixmap.scaled(150, 100, Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
    ui->imageFrame->setVisible(true);

    /*QTextDocumentFragment fragment;
    auto htmlStr = QString(
                R"delim(
                <div align="%1">
                   <img src="%2" width="%3" height="%4">
                </div>
                )delim").arg("center").arg(dir.absoluteFilePath(image.fileName))
                        .arg(image.width).arg(image.height);
    fragment = QTextDocumentFragment::fromHtml(htmlStr);
    auto cursor = myTextEdit->textCursor();
    cursor.movePosition(
                image.vertAlign == CaseImage::VertAlign::Top
                ? QTextCursor::Start : QTextCursor::End);

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignHCenter);
    cursor.insertBlock(blockFormat);


    QImage imageData(dir.absoluteFilePath(image.fileName));
    myTextEdit->document()->addResource(
                QTextDocument::ImageResource, QUrl(image.fileName), imageData);
    QTextImageFormat imageFormat;
    imageFormat.setName(image.fileName);
    imageFormat.setWidth(image.width);
    imageFormat.setHeight(image.height);
    cursor.insertImage(imageFormat);
    //cursor.insertFragment(fragment);*/
}

void TextEditorPage::resetImage()
{
    ui->imageFrame->setVisible(false);
}
