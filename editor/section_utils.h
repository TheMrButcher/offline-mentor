#ifndef SECTION_UTILS_H
#define SECTION_UTILS_H

#include <omkit/section.h>

void loadSections();
bool isKnownSection(QString path);
void addSection(const Section& section);
const QList<Section>& getSections();
Section getSection(QString path);
void updateSection(const Section& section);

#endif // SECTION_UTILS_H
