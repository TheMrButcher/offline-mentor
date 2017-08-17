#include "group.h"
#include "json_utils.h"
#include <QJsonArray>

Group::Group()
{
}

Group Group::createGroup()
{
    Group group;
    group.id = QUuid::createUuid();
    return group;
}

QList<Group> Group::load(QString path)
{
    QList<Group> result;
    QJsonObject rootObj;
    if (!readJSON(path, rootObj))
        return result;

    QJsonArray groups = rootObj["groups"].toArray();
    result.reserve(groups.size());
    for (const QJsonValue& value : groups) {
        Group group = Group::fromJson(value.toObject());
        if (group.isValid())
            result.append(group);
    }
    return result;
}

bool Group::save(const QList<Group>& groups, QString path)
{
    QJsonObject rootObj;
    QJsonArray groupsJSON;
    for (const auto& group : groups)
        groupsJSON.append(group.toJson());
    rootObj["groups"] = groupsJSON;
    return writeJSON(path, rootObj);
}

Group Group::fromJson(const QJsonObject& jsonObject)
{
    Group group;
    group.id = QUuid(jsonObject["id"].toString(""));
    group.name = jsonObject["name"].toString("");
    QJsonArray userNamesJSON = jsonObject["userNames"].toArray();
    for (const QJsonValue& value : userNamesJSON)
        group.userNames.insert(value.toString());
    return group;
}

QJsonObject Group::toJson() const
{
    QJsonObject result;
    result["id"] = id.toString();
    result["name"] = name;
    QJsonArray userNamesJSON;
    for (const auto& userName : userNames)
        userNamesJSON.append(userName);
    result["userNames"] = userNamesJSON;
    return result;
}

bool Group::isValid() const
{
    return !id.isNull() && !name.isEmpty();
}

void Group::sort()
{
    sortedUserNames = QList<QString>::fromSet(userNames);
    qSort(sortedUserNames);
}
