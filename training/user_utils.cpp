#include "user_utils.h"
#include <QDir>

namespace {
QString userNameValue;
}

QString userName()
{
    return userNameValue;
}

void setUserName(QString name)
{
    userNameValue = name;
}

QString getUserPath(QString path)
{
    QDir dir(path);
    if (!dir.exists(userNameValue))
        if (!dir.mkdir(userNameValue))
            return QString();
    return dir.absoluteFilePath(userNameValue);
}
