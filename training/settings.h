#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    static Settings& instance();

    bool read();
    void write() const;
    QString localDataPath() const;

    QString lastLogin;
    QString sectionsPath;
    QString solutionsPath;

private:
    Settings();
};

#endif // SETTINGS_H
