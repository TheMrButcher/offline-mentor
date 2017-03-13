#ifndef SECTION_H
#define SECTION_H

#include <QString>

class Section
{
public:
    Section();

    bool open();
    bool save();

    QString name;
    QString path;
};

#endif // SECTION_H
