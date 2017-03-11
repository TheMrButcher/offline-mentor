#include "settings.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

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
    QFile settingsFile(SETTINGS_FILE_NAME);
    if (!settingsFile.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError errors;
    QJsonDocument json = QJsonDocument::fromJson(settingsFile.readAll(), &errors);
    if (errors.error != QJsonParseError::NoError)
        return false;
    if (!json.isObject())
        return false;

    auto rootObj = json.object();
    lastLogin = rootObj["lastLogin"].toString(lastLogin);
    return true;
}

void Settings::write() const
{
    QFile settingsFile(SETTINGS_FILE_NAME);
    if (!settingsFile.open(QIODevice::WriteOnly))
        return;

    QJsonObject rootObj;
    rootObj["lastLogin"] = lastLogin;

    QJsonDocument json(rootObj);
    settingsFile.write(json.toJson());
}

Settings::Settings()
{}
