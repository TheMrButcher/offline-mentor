#ifndef SECTION_H
#define SECTION_H

#include "case.h"

#include <QString>
#include <QList>
#include <QDir>

class Section
{
public:
    Section();

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
