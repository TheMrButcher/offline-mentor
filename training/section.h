#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QList>

class Section
{
public:
    Section();

    static QList<Section> loadSections();

    QString name;
    QString directoryName;
};

#endif // SECTION_H
