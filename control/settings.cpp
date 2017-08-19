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
    sectionsPath = rootObj["sectionsPath"].toString(sectionsPath);
    solutionsPath = rootObj["solutionsPath"].toString(solutionsPath);
    groupsPath = rootObj["groupsPath"].toString(groupsPath);
    lastPath = rootObj["lastPath"].toString(lastPath);
    isFirstUsage = rootObj["isFirstUsage"].toBool(false);
    return true;
}

bool Settings::write() const
{
    QJsonObject rootObj;
    rootObj["sectionsPath"] = sectionsPath;
    rootObj["solutionsPath"] = solutionsPath;
    rootObj["groupsPath"] = groupsPath;
    rootObj["lastPath"] = lastPath;
    rootObj["isFirstUsage"] = false;
    return writeJSON(SETTINGS_FILE_NAME, rootObj);
}

QString Settings::localDataPath() const
{
    QDir dir;
    if (!dir.exists("localData"))
        if (!dir.mkdir("localData"))
            return QString();
    return dir.absoluteFilePath("localData");
}

QString Settings::localSolutionsPath() const
{
    QString path = localDataPath();
    if (path.isEmpty())
        return QString();
    QDir dir(path);
    if (!dir.exists("solutions"))
        if (!dir.mkdir("solutions"))
            return QString();
    return dir.absoluteFilePath("solutions");
}

QString Settings::localGroupsPath() const
{
    QString path = localDataPath();
    if (path.isEmpty())
        return QString();
    QDir dir(path);
    return dir.absoluteFilePath("Groups.json");
}

bool Settings::isNetworkSupported() const
{
    return !solutionsPath.isEmpty();
}

void Settings::updateLastPath(QString newPath)
{
    lastPath = newPath;
    write();
}

Settings::Settings()
    : isFirstUsage(false)
{}
