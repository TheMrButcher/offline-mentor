#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "omkit_global.h"

#include <QString>
#include <QJsonObject>

OMKITSHARED_EXPORT bool readJSON(QString fileName, QJsonObject& jsonData);
OMKITSHARED_EXPORT bool writeJSON(QString fileName, const QJsonObject& jsonData);

#endif // JSON_UTILS_H
