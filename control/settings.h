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

    QString sectionsPath;
    QString solutionsPath;

private:
    Settings();
};

#endif // SETTINGS_H
