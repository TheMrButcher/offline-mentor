#include "utils.h"

#include <QFile>
#include <QDir>

QString getVersion()
{
    QFile versionFile(":/VERSION.txt");
    versionFile.open(QIODevice::ReadOnly);
    return QString(versionFile.readAll());
}

QString getNewDir(QString path, QString dirNamePrefix)
{
    QDir dir(path);
    if (!dir.exists(dirNamePrefix)) {
        if (dir.mkdir(dirNamePrefix))
            return dir.absoluteFilePath(dirNamePrefix);
    }

    for (int i = 0; i < 100; ++i) {
        QString dirName = QString("%1_%2").arg(dirNamePrefix).arg(i);
        if (!dir.exists(dirName)) {
            if (dir.mkdir(dirName))
                return dir.absoluteFilePath(dirName);
        }
    }
    return QString();
}
