#include "settings.h"

namespace {
const QString SETTINGS_FILE_NAME = "Settings.json";
}

Settings& Settings::instance()
{
    static Settings settings;
    return settings;
}

void Settings::setLastLogin(QString login)
{
    lastFirstName = "";
    lastSurname = "";
    lastLogin = login;
    write();
}

void Settings::setLastLogin(QString firstName, QString surname, QString login)
{
    lastFirstName = firstName;
    lastSurname = surname;
    lastLogin = login;
    write();
}

Settings::Settings()
    : TrainingSettings(SETTINGS_FILE_NAME)
{}
