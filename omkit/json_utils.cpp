#include "json_utils.h"
#include <QFile>
#include <QJsonDocument>

bool readJSON(QString fileName, QJsonObject& jsonData)
{
    if (fileName.isEmpty())
        return false;
    QFile file(fileName);
    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError errors;
    QJsonDocument json = QJsonDocument::fromJson(file.readAll(), &errors);
    if (errors.error != QJsonParseError::NoError)
        return false;
    if (!json.isObject())
        return false;

    jsonData = json.object();
    return true;
}

bool writeJSON(QString fileName, const QJsonObject& jsonData)
{
    if (fileName.isEmpty())
        return false;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QJsonDocument json(jsonData);
    return file.write(json.toJson()) != -1;
}
