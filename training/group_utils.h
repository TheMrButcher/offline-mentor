#ifndef GROUP_UTILS_H
#define GROUP_UTILS_H

#include <omkit/group.h>

void loadGroups();
const QList<Group>& getGroups();
const Group& getGroup(const QUuid& id);

#endif // GROUP_UTILS_H
