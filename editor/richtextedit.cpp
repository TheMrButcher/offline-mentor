#include "richtextedit.h"
#include <QMimeData>
#include <QRegExp>

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
{}

void RichTextEdit::clearFormat()
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontUnderline(false);
    format.setFontStrikeOut(false);
    format.setFontItalic(false);
    format.setFontFamily("Times New Roman");
    format.setFontPointSize(12);
    format.setFontLetterSpacing(100);
    format.clearBackground();
    format.clearForeground();
    applyFormat(format);
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

void RichTextEdit::applyFormat(const QTextCharFormat& format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}
