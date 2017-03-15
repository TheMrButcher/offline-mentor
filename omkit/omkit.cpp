#include "omkit.h"
#include "utils.h"

#include <QFile>

OMKit& OMKit::instance()
{
    static OMKit kit;
    return kit;
}

QString OMKit::getVersion()
{
    QFile versionFile(":/OMK_VERSION.txt");
    versionFile.open(QIODevice::ReadOnly);
    return QString(versionFile.readAll());
}

OMKit::OMKit()
{
}
