#ifndef SECTION_UTILS_H
#define SECTION_UTILS_H

#include <omkit/section.h>

#include <QHash>

void loadSections();

const QHash<QUuid, Section>& getSections();

#endif // SECTION_UTILS_H
