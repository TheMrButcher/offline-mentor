#include "richtextedit.h"
#include <QMimeData>
#include <QRegExp>

#include <QDebug>

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
