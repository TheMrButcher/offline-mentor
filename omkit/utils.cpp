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

bool copyDir(QString srcPath, QString dstPath)
{
    QDir srcDir(srcPath);
    QDir dstDir(dstPath);
    return copyDir(srcDir, dstDir);
}

bool copyDir(QDir srcDir, QDir dstDir)
{
    if (!srcDir.exists() || !dstDir.exists())
        return false;
    auto entries = srcDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const auto& entry, entries) {
        auto name = entry.fileName();
        auto srcFilePath = srcDir.absoluteFilePath(name);
        auto dstFilePath = dstDir.absoluteFilePath(name);
        if (entry.isDir()) {
            dstDir.mkdir(name);
            if (!copyDir(srcFilePath, dstFilePath))
                return false;
        } else {
            if (!QFile::copy(srcFilePath, dstFilePath))
                return false;
        }
    }
    return true;
}
