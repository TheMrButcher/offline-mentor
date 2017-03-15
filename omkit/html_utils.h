#ifndef HTML_UTILS_H
#define HTML_UTILS_H

#include "omkit_global.h"

#include <QString>

class QTextDocument;

OMKITSHARED_EXPORT QString readHTML(QString fileName);
OMKITSHARED_EXPORT bool writeHTML(QString fileName, QTextDocument* document);

#endif // HTML_UTILS_H
