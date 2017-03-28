#ifndef SECTION_H
#define SECTION_H

#include "omkit_global.h"
#include "case.h"

#include <QString>
#include <QList>
#include <QDir>
#include <QUuid>

class OMKITSHARED_EXPORT Section
{
public:
    Section();

    static Section createSection(QString path);
    static QList<Section> findAll(QString path);

    bool isValid() const;
    bool open();
    bool save() const;
    Section saveAs(QString newPath) const;
    QString nextCaseFilePrefix();
    QDir dir() const;
    void copyHidden(const Section& section);
    QString makeTotalFileName();

    QUuid id;
    QString name;
    QString description;
    QString path;
    QString totalFileName;
    QList<Case> cases;
    int nextIndex;
};

#endif // SECTION_H
