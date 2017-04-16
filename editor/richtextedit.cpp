#include "richtextedit.h"
#include <QMimeData>
#include <QRegExp>
#include <QTextList>

namespace {
QRegExp imgRegExp =
[]()
{
    QRegExp result("\\<img[^\\>]*src\\s*=\\s*\"([^\"]*)\"[^\\>]*\\>", Qt::CaseInsensitive);
    result.setMinimal(true);
    return result;
}();
}

RichTextEdit::RichTextEdit(QWidget* parent)
    : QTextEdit(parent)
{
    setFont(QFont("Times New Roman", 14));

    connect(this, SIGNAL(textChanged()), this, SLOT(updateCursor()));
}

void RichTextEdit::clearFormat()
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontUnderline(false);
    format.setFontStrikeOut(false);
    format.setFontItalic(false);
    format.setFontFamily("Times New Roman");
    format.setFontPointSize(14);
    format.setFontLetterSpacing(100);
    format.clearBackground();
    format.clearForeground();
    applyFormat(format);
}

void RichTextEdit::applyFormat(const QTextCharFormat& format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

void RichTextEdit::clearListFormat()
{
    QTextCursor cursor = textCursor();
    QTextList* textList = cursor.currentList();
    if (!textList)
        return;
    cursor.beginEditBlock();
    textList->remove(cursor.block());
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setIndent(0);
    cursor.setBlockFormat(blockFormat);
    cursor.endEditBlock();
}

void RichTextEdit::applyListStyle(QTextListFormat::Style style)
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    QTextListFormat format;
    if (QTextList* textList = cursor.currentList())
        format = textList->format();
    format.setStyle(style);
    cursor.createList(format);
    cursor.endEditBlock();
}

void RichTextEdit::updateCursor()
{
    QTextCursor cursor = textCursor();
    if (cursor.position() == 0 && cursor.anchor() == 0) {
        if (document()->isEmpty()) {
            clearFormat();
        } else {
            cursor.setPosition(1);
            setTextCursor(cursor);
            cursor.setPosition(0);
            setTextCursor(cursor);
        }
    }
}

void RichTextEdit::insertFromMimeData(const QMimeData* source)
{
    QMimeData* filteredSource = new QMimeData;
    if (source->hasText())
        filteredSource->setText(source->text());
    if (source->hasHtml()) {
        QString html = source->html();
        html.remove(imgRegExp);
        filteredSource->setHtml(html);
    }
    if (source->hasUrls())
        filteredSource->setUrls(source->urls());
    QTextEdit::insertFromMimeData(filteredSource);
    delete filteredSource;
}
