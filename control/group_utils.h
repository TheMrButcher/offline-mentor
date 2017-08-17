#ifndef GROUP_UTILS_H
#define GROUP_UTILS_H

#include <omkit/group.h>

void loadGroups();
const QList<Group>& getGroups();
const Group& getGroup(const QUuid& id);
void removeGroup(const QUuid& id);
void addGroup(const Group& group);
const QList<const Group*>& getGroupsByUserName(QString userName);

#endif // GROUP_UTILS_H
