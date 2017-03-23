#include "omkit.h"
#include "utils.h"
#include <QFile>
#include <quazip.h>

OMKit& OMKit::instance()
{
    static OMKit kit;
    return kit;
}

void OMKit::init()
{
    QuaZip::setDefaultFileNameCodec("cp866");
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
