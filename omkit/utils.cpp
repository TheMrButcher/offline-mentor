#include "utils.h"

#include <QFile>

QString getVersion()
{
    QFile versionFile(":/VERSION.txt");
    versionFile.open(QIODevice::ReadOnly);
    return QString(versionFile.readAll());
}
