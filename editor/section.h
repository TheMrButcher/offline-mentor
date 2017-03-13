#ifndef SECTION_H
#define SECTION_H

#include <QString>

class Section
{
public:
    Section();

    bool create();

    QString name;
    QString path;
};

#endif // SECTION_H
