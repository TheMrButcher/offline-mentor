#include "section.h"

#include <QFile>
#include <QFileInfo>
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
    nextIndex = rootObj["nextIndex"].toInt(1);

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
    rootObj["nextIndex"] = nextIndex;

    QJsonArray casesArray;
    foreach (auto c, cases)
        casesArray.append(c.toJson());
    rootObj["cases"] = casesArray;

    QJsonDocument json(rootObj);
    sectionFile.write(json.toJson());
    return true;
}

QString Section::nextCaseFilePrefix()
{
    QFileInfo fileInfo(path);
    QString baseName = fileInfo.baseName();
    QDir sectionDir = dir();
    for (int i = 0; i < 100; ++i) {
        QString caseFilePrefix = baseName + " Кейс" + QString::number(nextIndex++);
        if (!sectionDir.exists(Case::makeQuestionFileName(caseFilePrefix))
            && !sectionDir.exists(Case::makeAnswerFileName(caseFilePrefix)))
            return caseFilePrefix;
    }
    return QString();
}

QDir Section::dir()
{
    return QFileInfo(path).dir();
}
