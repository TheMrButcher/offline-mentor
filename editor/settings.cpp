#include "settings.h"
#include "section.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
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
    QFile settingsFile(SETTINGS_FILE_NAME);
    if (!settingsFile.open(QIODevice::ReadOnly))
        return;
    QJsonParseError errors;
    QJsonDocument json = QJsonDocument::fromJson(settingsFile.readAll(), &errors);
    if (errors.error != QJsonParseError::NoError)
        return;
    if (!json.isObject())
        return;

    auto rootObj = json.object();
    lastDirectoryPath = rootObj["lastDirectoryPath"].toString(lastDirectoryPath);

    QJsonArray knownSectionsArray = rootObj["knownSections"].toArray();
    knownSections.clear();
    foreach (auto knownSectionValue, knownSectionsArray) {
        knownSections.append(knownSectionValue.toString());
    }
}

void Settings::write()
{
    QFile settingsFile(SETTINGS_FILE_NAME);
    if (!settingsFile.open(QIODevice::WriteOnly))
        return;

    QJsonObject rootObj;
    rootObj["lastDirectoryPath"] = lastDirectoryPath;

    QJsonArray knownSectionsArray;
    foreach (auto knownSection, knownSections)
        knownSectionsArray.append(knownSection);
    rootObj["knownSections"] = knownSectionsArray;

    QJsonDocument json(rootObj);
    settingsFile.write(json.toJson());
}

QString Settings::safeLastDirectoryPath()
{
    if (!QFileInfo(lastDirectoryPath).isDir())
        lastDirectoryPath = QDir::homePath();
    return lastDirectoryPath;
}

Settings::Settings()
{
    lastDirectoryPath = QDir::homePath();
}
