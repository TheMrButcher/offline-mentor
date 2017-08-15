#include "html_utils.h"
#include <QFile>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QTextDocumentFragment>
#include <QTextBlockFormat>
#include <QTextImageFormat>
#include <QTextEdit>

QString readHTML(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QString();

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (!Qt::mightBeRichText(str))
        return QString();
    return str;
}

bool writeHTML(QString fileName, QTextDocument* document)
{
    QTextDocumentWriter writer(fileName);
    return writer.write(document);
}

void setImageAndHTML(
        QDir dir, const CaseImage& image, QString html, QTextEdit* textEdit)
{
    if (image.isEmpty()) {
        textEdit->setHtml(html);
        return;
    }
    if (image.vertAlign == CaseImage::VertAlign::Bottom) {
        textEdit->setHtml(html);
        addImage(dir, image, textEdit);
        textEdit->moveCursor(QTextCursor::Start);
        return;
    }
    auto cursor = textEdit->textCursor();
    cursor.insertBlock();
    cursor.insertFragment(QTextDocumentFragment::fromHtml(html));
    addImage(dir, image, textEdit);
    textEdit->moveCursor(QTextCursor::Start);
}

void addImage(QDir dir, const CaseImage& image, QTextEdit* textEdit)
{
    if (image.isEmpty())
        return;

    auto cursor = textEdit->textCursor();
    cursor.movePosition(
                image.vertAlign == CaseImage::VertAlign::Top
                ? QTextCursor::Start : QTextCursor::End);

    QTextBlockFormat blockFormat;
    switch (image.horAlign) {
    case CaseImage::HorAlign::Left: blockFormat.setAlignment(Qt::AlignLeft); break;
    case CaseImage::HorAlign::Center: blockFormat.setAlignment(Qt::AlignHCenter); break;
    case CaseImage::HorAlign::Right: blockFormat.setAlignment(Qt::AlignRight); break;
    }
    if (image.vertAlign == CaseImage::VertAlign::Top)
        blockFormat.setBottomMargin(15);
    if (image.vertAlign == CaseImage::VertAlign::Bottom)
        blockFormat.setTopMargin(15);
    cursor.insertBlock(blockFormat);

    QImage imageData(dir.absoluteFilePath(image.fileName));
    if (image.width != imageData.width() || image.height != imageData.height()) {
        imageData = imageData.scaled(image.width, image.height, Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation);
    }
    textEdit->document()->addResource(
                QTextDocument::ImageResource, QUrl(image.fileName), imageData);
    QTextImageFormat imageFormat;
    imageFormat.setName(image.fileName);
    imageFormat.setWidth(image.width);
    imageFormat.setHeight(image.height);
    cursor.insertImage(imageFormat);
}
