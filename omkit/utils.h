#ifndef UTILS_H
#define UTILS_H

#include "omkit_global.h"

#include <QString>
#include <QDir>

OMKITSHARED_EXPORT QString getVersion();

OMKITSHARED_EXPORT QString getNewDir(QString path, QString dirNamePrefix);

OMKITSHARED_EXPORT bool copyDir(QString srcPath, QString dstPath);
OMKITSHARED_EXPORT bool copyDir(QDir srcDir, QDir dstDir);

#endif // UTILS_H
