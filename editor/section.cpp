#include "section.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

    QJsonArray casesArray = rootObj["cases"].toArray();
    cases.clear();
    foreach (auto caseValue, casesArray)
        cases.append(Case::fromJson(caseValue.toObject()));

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

    QJsonArray casesArray;
    foreach (auto c, cases)
        casesArray.append(c.toJson());
    rootObj["cases"] = casesArray;

    QJsonDocument json(rootObj);
    sectionFile.write(json.toJson());
    return true;
}
