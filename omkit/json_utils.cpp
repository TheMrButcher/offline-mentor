#include "json_utils.h"
#include <QFile>
#include <QJsonDocument>

bool readJSON(QString fileName, QJsonObject& jsonData)
{
    QFile settingsFile(fileName);
    if (!settingsFile.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError errors;
    QJsonDocument json = QJsonDocument::fromJson(settingsFile.readAll(), &errors);
    if (errors.error != QJsonParseError::NoError)
        return false;
    if (!json.isObject())
        return false;

    jsonData = json.object();
    return true;
}

bool writeJSON(QString fileName, const QJsonObject& jsonData)
{
    QFile settingsFile(fileName);
    if (!settingsFile.open(QIODevice::WriteOnly))
        return false;
    QJsonDocument json(jsonData);
    return settingsFile.write(json.toJson()) != -1;
}
