#include "utils.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>

QString getVersion()
{
    QFile versionFile(":/VERSION.txt");
    versionFile.open(QIODevice::ReadOnly);
    return QString(versionFile.readAll());
}

QDir getDir(QString path)
{
    QDir dir(path);
    if (dir.exists())
        return dir;
    QDir().mkpath(path);
    return dir;
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

QString getNewFileName(QString dirPath, QString oldFilePath)
{
    QFileInfo fileInfo(oldFilePath);
    QString fileName = fileInfo.fileName();
    QDir dir(dirPath);
    if (!dir.exists(fileName))
        return dir.absoluteFilePath(fileName);

    QString baseFileName = fileInfo.baseName();
    QString suffix = fileInfo.completeSuffix();
    for (int i = 0; i < 100; ++i) {
        QString newFileName = QString("%1_%2.%3").arg(baseFileName).arg(i).arg(suffix);
        if (!dir.exists(newFileName))
            return dir.absoluteFilePath(newFileName);
    }
    return QString();
}

bool isDirEmpty(QString path)
{
    return isDirEmpty(QDir(path));
}

bool isDirEmpty(QDir dir)
{
    return dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden).isEmpty();
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

bool copyWithOverwrite(QString srcPath, QString dstPath)
{
    if (QFile::exists(dstPath)) {
        if (!QFile::remove(dstPath))
            return false;
    }
    return QFile::copy(srcPath, dstPath);
}
