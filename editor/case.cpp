#include "case.h"

Case::Case()
{}

Case Case::fromJson(const QJsonObject& jsonObject)
{
    Case c;
    c.name = jsonObject["name"].toString();
    return c;
}

QJsonObject Case::toJson() const
{
    QJsonObject result;
    result["name"] = name;
    return result;
}
