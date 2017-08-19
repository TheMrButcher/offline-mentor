#include "group_utils.h"
#include "solution_utils.h"
#include "settings.h"
#include <QDir>

namespace {
QList<Group> groups;
QHash<QUuid, const Group*> groupMap;
QHash<QString, QList<const Group*>> userToGroupMap;
const Group BAD_GROUP;
const QList<const Group*> EMPTY_GROUP_LIST;

void updateAll()
{
    groupMap.clear();
    userToGroupMap.clear();
    for (auto& group : groups) {
        groupMap[group.id] = &group;
        if (group.sortedUserNames.size() != group.userNames.size())
            group.sort();
        for (const auto& userName : group.userNames)
            userToGroupMap[userName].append(&group);
    }
    updateUserNamesWithoutGroup();
}

void saveGroups()
{
    updateAll();
    const auto& settings = Settings::instance();
    Group::save(groups, settings.localGroupsPath());
    if (!settings.groupsPath.isEmpty())
        Group::save(groups, settings.groupsPath);
}
}

void loadGroups()
{
    groups.clear();
    const auto& settings = Settings::instance();
    if (!settings.groupsPath.isEmpty()) {
        QHash<QUuid, Group> allGroupMap;
        if (QDir().exists(settings.groupsPath)) {
            QList<Group> remoteGroups = Group::load(settings.groupsPath);
            for (const auto& group : remoteGroups)
                allGroupMap[group.id] = group;
        }
        if (QFile(settings.localGroupsPath()).exists()) {
            QList<Group> localGroups = Group::load(settings.localGroupsPath());
            for (const auto& group : localGroups)
                allGroupMap[group.id] = group;
        }
        groups = allGroupMap.values();
        saveGroups();
        return;
    }
    if (QFile(settings.localGroupsPath()).exists())
        groups = Group::load(settings.localGroupsPath());
    updateAll();
}

const QList<Group>& getGroups()
{
    return groups;
}

const Group& getGroup(const QUuid& id)
{
    auto it = groupMap.find(id);
    if (it == groupMap.end())
        return BAD_GROUP;
    return *it.value();
}

void removeGroup(const QUuid& id)
{
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        if (it->id == id) {
            groups.erase(it);
            saveGroups();
            return;
        }
    }
}

void addGroup(const Group& group)
{
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        if (it->id == group.id) {
            *it = group;
            it->sortedUserNames.clear();
            saveGroups();
            return;
        }
    }
    groups.append(group);
    saveGroups();
}

const QList<const Group*>& getGroupsByUserName(QString userName)
{
    auto it = userToGroupMap.find(userName);
    if (it == userToGroupMap.end())
        return EMPTY_GROUP_LIST;
    return it.value();
}
