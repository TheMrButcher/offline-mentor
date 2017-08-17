#ifndef GROUP_H
#define GROUP_H

#include "omkit_global.h"
#include <QSet>
#include <QList>
#include <QString>
#include <QUuid>
#include <QJsonObject>

class OMKITSHARED_EXPORT Group
{
public:
    Group();

    static Group createGroup();
    static QList<Group> load(QString path);
    static bool save(const QList<Group>& groups, QString path);

    static Group fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;

    bool isValid() const;
    void sort();

    QUuid id;
    QString name;
    QSet<QString> userNames;
    QList<QString> sortedUserNames;
};

#endif // GROUP_H
