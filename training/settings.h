#ifndef SETTINGS_H
#define SETTINGS_H

#include <omkit/trainingsettings.h>

class Settings : public TrainingSettings
{
public:
    static Settings& instance();

    void setLastLogin(QString login);
    void setLastLogin(QString firstName, QString surname, QString login);

private:
    Settings();
};

#endif // SETTINGS_H
