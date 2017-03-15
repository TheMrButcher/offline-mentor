#include "settings.h"
#include <omkit/json_utils.h>

namespace {
const QString SETTINGS_FILE_NAME = "Settings.json";
}

Settings&Settings::instance()
{
    static Settings settings;
    return settings;
}

bool Settings::read()
{
    QJsonObject rootObj;
    if (!readJSON(SETTINGS_FILE_NAME, rootObj))
        return false;
    lastLogin = rootObj["lastLogin"].toString(lastLogin);
    sectionsPath = rootObj["sectionsPath"].toString(sectionsPath);
    return true;
}

void Settings::write() const
{
    QJsonObject rootObj;
    rootObj["lastLogin"] = lastLogin;
    rootObj["sectionsPath"] = sectionsPath;
    writeJSON(SETTINGS_FILE_NAME, rootObj);
}

Settings::Settings()
{}
