#include "group_utils.h"
#include "settings.h"
#include <QDir>

namespace {
QList<Group> groups;
QHash<QUuid, const Group*> groupMap;
const Group BAD_GROUP;

void updateAll()
{
    groupMap.clear();
    for (auto& group : groups) {
        groupMap[group.id] = &group;
        if (group.sortedUserNames.size() != group.userNames.size())
            group.sort();
    }
}

void saveGroups()
{
    Group::save(groups, Settings::instance().localGroupsPath());
}

QList<Group> filterGroups(const QList<Group>& groups)
{
    QList<Group> filteredGroups;
    auto allowedIds = QSet<QUuid>::fromList(Settings::instance().customGroups);
    for (const auto& group : groups) {
        if (allowedIds.contains(group.id))
            filteredGroups.append(group);
    }
    return filteredGroups;
}
}

void loadGroups()
{
    groups.clear();
    const auto& settings = Settings::instance();
    if (!settings.groupsPath.isEmpty()) {
        QHash<QUuid, Group> allGroupMap;
        if (QFile(settings.localGroupsPath()).exists()) {
            QList<Group> localGroups = Group::load(settings.localGroupsPath());
            for (const auto& group : localGroups)
                allGroupMap[group.id] = group;
        }
        if (QDir().exists(settings.groupsPath)) {
            QList<Group> remoteGroups = Group::load(settings.groupsPath);
            for (const auto& group : remoteGroups)
                allGroupMap[group.id] = group;
        }
        groups = allGroupMap.values();
        if (!settings.areAllGroupsAllowed)
            groups = filterGroups(groups);
        updateAll();
        saveGroups();
        return;
    }
    if (QFile(settings.localGroupsPath()).exists())
        groups = Group::load(settings.localGroupsPath());
    if (!settings.areAllGroupsAllowed)
        groups = filterGroups(groups);
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
