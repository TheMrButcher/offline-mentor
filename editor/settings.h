#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QList>

class Settings
{
public:
    static Settings& instance();

    void read();
    void write();
    QString safeLastDirectoryPath();
    void updateLastDirectoryPath(QString path);

    QString lastDirectoryPath;
    QList<QString> knownSections;

private:
    Settings();
};

#endif // SETTINGS_H
