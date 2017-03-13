#include "section.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Section::Section()
{}

bool Section::create()
{
    QFile sectionFile(path);
    if (!sectionFile.open(QIODevice::WriteOnly))
        return false;

    QJsonObject rootObj;
    rootObj["name"] = name;

    QJsonDocument json(rootObj);
    sectionFile.write(json.toJson());
    return true;
}
