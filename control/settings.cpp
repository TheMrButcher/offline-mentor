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
    lastPath = rootObj["lastPath"].toString(lastPath);
    return true;
}

bool Settings::write() const
{
    QJsonObject rootObj;
    rootObj["sectionsPath"] = sectionsPath;
    rootObj["solutionsPath"] = solutionsPath;
    rootObj["lastPath"] = lastPath;
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

void Settings::updateLastPath(QString newPath)
{
    lastPath = newPath;
    write();
}

Settings::Settings()
{}
