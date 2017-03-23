#include "string_utils.h"
#include <QStringList>

QString trim(QString str, int len)
{
    if (str.size() < len)
        return str;
    QStringList parts = str.split(' ', QString::SkipEmptyParts);
    if (parts.isEmpty())
        return QString();
    str = parts.front();
    int index = 1;
    while (index < parts.size() && str.size() + parts[index].size() + 1 <= len)
        str += " " + parts[index++];
    if (str.size() > len)
        str = str.left(len);
    return str.left(len) + "...";
}
