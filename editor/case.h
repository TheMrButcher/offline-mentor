#ifndef CASE_H
#define CASE_H

#include <QJsonObject>
#include <QString>

class Case
{
public:
    Case();

    static Case fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;

    QString name;
};

#endif // CASE_H
