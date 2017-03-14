#ifndef SECTION_H
#define SECTION_H

#include "case.h"

#include <QString>
#include <QList>

class Section
{
public:
    Section();

    bool open();
    bool save();

    QString name;
    QString description;
    QString path;
    QList<Case> cases;
};

#endif // SECTION_H
