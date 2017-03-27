#include "settings.h"

#include <omkit/json_utils.h>
#include <omkit/section.h>

#include <QDir>
#include <QFileInfo>
#include <QJsonArray>

namespace {
const QString SETTINGS_FILE_NAME = "Settings.json";
}

Settings& Settings::instance()
{
    static Settings settings;
    return settings;
}

void Settings::read()
{
    QJsonObject rootObj;
    if (!readJSON(SETTINGS_FILE_NAME, rootObj))
        return;

    lastDirectoryPath = rootObj["lastDirectoryPath"].toString(lastDirectoryPath);

    QJsonArray knownSectionsArray = rootObj["knownSections"].toArray();
    knownSections.clear();
    foreach (auto knownSectionValue, knownSectionsArray) {
        knownSections.append(knownSectionValue.toString());
    }
}

void Settings::write()
{
    QJsonObject rootObj;
    rootObj["lastDirectoryPath"] = lastDirectoryPath;

    QJsonArray knownSectionsArray;
    foreach (auto knownSection, knownSections)
        knownSectionsArray.append(knownSection);
    rootObj["knownSections"] = knownSectionsArray;

    writeJSON(SETTINGS_FILE_NAME, rootObj);
}

QString Settings::safeLastDirectoryPath()
{
    if (!QFileInfo(lastDirectoryPath).isDir())
        lastDirectoryPath = QDir::homePath();
    return lastDirectoryPath;
}

void Settings::updateLastDirectoryPath(QString path)
{
    if (path.isEmpty())
        return;
    QFileInfo fileInfo(path);
    if (fileInfo.isDir())
        lastDirectoryPath = fileInfo.absoluteFilePath();
    else
        lastDirectoryPath = fileInfo.absolutePath();
}

Settings::Settings()
{
    lastDirectoryPath = QDir::homePath();
}
