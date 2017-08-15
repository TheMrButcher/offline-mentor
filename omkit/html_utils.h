#ifndef HTML_UTILS_H
#define HTML_UTILS_H

#include "omkit_global.h"
#include "caseimage.h"
#include <QString>
#include <QDir>

class QTextDocument;
class QTextEdit;

OMKITSHARED_EXPORT QString readHTML(QString fileName);
OMKITSHARED_EXPORT bool writeHTML(QString fileName, QTextDocument* document);
OMKITSHARED_EXPORT void setImageAndHTML(
        QDir dir, const CaseImage& image, QString html, QTextEdit* textEdit);
OMKITSHARED_EXPORT void addImage(
        QDir dir, const CaseImage& image, QTextEdit* textEdit);

#endif // HTML_UTILS_H
