#ifndef SECTION_UTILS_H
#define SECTION_UTILS_H

#include <omkit/section.h>

#include <QHash>
#include <QStringList>

void loadSections();
const QHash<QUuid, Section>& getSections();
const QList<Section>& getSortedSections();
const QStringList& getSectionNames();
QStringList importSectionsFromFolder(QString path);
QStringList importSectionsFromArchive(QString path);

#endif // SECTION_UTILS_H
