#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    static Settings& instance();

    bool read();
    bool write() const;
    QString localDataPath() const;
    QString localSolutionsPath() const;
    void updateLastPath(QString newPath);

    QString sectionsPath;
    QString solutionsPath;
    QString lastPath;
    bool isFirstUsage;

private:
    Settings();
};

#endif // SETTINGS_H
