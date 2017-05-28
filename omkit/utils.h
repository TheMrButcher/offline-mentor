#ifndef UTILS_H
#define UTILS_H

#include "omkit_global.h"

#include <QString>
#include <QDir>

OMKITSHARED_EXPORT QString getVersion();
OMKITSHARED_EXPORT QDir getDir(QString path);
OMKITSHARED_EXPORT QString getNewDir(QString path, QString dirNamePrefix);
OMKITSHARED_EXPORT bool isDirEmpty(QString path);
OMKITSHARED_EXPORT bool isDirEmpty(QDir dir);

OMKITSHARED_EXPORT bool copyDir(QString srcPath, QString dstPath);
OMKITSHARED_EXPORT bool copyDir(QDir srcDir, QDir dstDir);

OMKITSHARED_EXPORT bool copyWithOverwrite(QString srcPath, QString dstPath);

#endif // UTILS_H
