#ifndef SECTION_H
#define SECTION_H

#include "omkit_global.h"
#include "case.h"

#include <QString>
#include <QList>
#include <QDir>

class OMKITSHARED_EXPORT Section
{
public:
    Section();

    static QList<Section> findAll(QString path);

    bool open();
    bool save();
    QString nextCaseFilePrefix();
    QDir dir();

    QString name;
    QString description;
    QString path;
    QList<Case> cases;
    int nextIndex;
};

#endif // SECTION_H
