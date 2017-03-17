#include "settings.h"
#include <omkit/json_utils.h>
#include <QDir>

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
    solutionsPath = rootObj["solutionsPath"].toString(solutionsPath);
    return true;
}

void Settings::write() const
{
    QJsonObject rootObj;
    rootObj["lastLogin"] = lastLogin;
    rootObj["sectionsPath"] = sectionsPath;
    rootObj["solutionsPath"] = solutionsPath;
    writeJSON(SETTINGS_FILE_NAME, rootObj);
}

QString Settings::localDataPath() const
{
    QDir dir;
    if (!dir.exists("localData"))
        if (!dir.mkdir("localData"))
            return QString();
    return dir.absoluteFilePath("localData");
}

Settings::Settings()
{}
