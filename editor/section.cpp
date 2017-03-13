#include "section.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Section::Section()
{}

bool Section::open()
{
    QFile sectionFile(path);
    if (!sectionFile.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError errors;
    QJsonDocument json = QJsonDocument::fromJson(sectionFile.readAll(), &errors);
    if (errors.error != QJsonParseError::NoError)
        return false;
    if (!json.isObject())
        return false;

    auto rootObj = json.object();
    name = rootObj["name"].toString("");
    if (name.isEmpty())
        return false;
    description = rootObj["description"].toString("");
    return true;
}

bool Section::save()
{
    QFile sectionFile(path);
    if (!sectionFile.open(QIODevice::WriteOnly))
        return false;

    QJsonObject rootObj;
    rootObj["name"] = name;
    rootObj["description"] = description;

    QJsonDocument json(rootObj);
    sectionFile.write(json.toJson());
    return true;
}
