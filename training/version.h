#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QFile>

QString getVersion()
{
    QFile versionFile(":/VERSION.txt");
    versionFile.open(QIODevice::ReadOnly);
    return QString(versionFile.readAll());
}

#endif // VERSION_H
