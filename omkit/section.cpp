#include "section.h"
#include "json_utils.h"
#include <QFileInfo>
#include <QJsonArray>

Section::Section()
{}

QList<Section> Section::findAll(QString /*path*/)
{
    QList<Section> result;
    return result;
}

bool Section::open()
{
    QJsonObject rootObj;
    if (!readJSON(path, rootObj))
        return false;
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
    QJsonObject rootObj;
    rootObj["name"] = name;
    rootObj["description"] = description;
    rootObj["nextIndex"] = nextIndex;

    QJsonArray casesArray;
    foreach (auto c, cases)
        casesArray.append(c.toJson());
    rootObj["cases"] = casesArray;

    return writeJSON(path, rootObj);
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
