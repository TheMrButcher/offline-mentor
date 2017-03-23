#include "section_utils.h"
#include "settings.h"

namespace {
QHash<QUuid, Section> sections;
QList<Section> sortedSections;
QStringList sectionNames;
}

void loadSections()
{
    sections.clear();
    sectionNames.clear();

    auto sectionList = Section::findAll(Settings::instance().sectionsPath);
    foreach (const auto& section, sectionList) {
        sections[section.id] = section;
        sectionNames.append(section.name);
        sortedSections.append(section);
    }

    qSort(sortedSections.begin(), sortedSections.end(),
          [](const Section& s1, const Section& s2) { return s1.name < s2.name; });
    qSort(sectionNames);
}

const QHash<QUuid, Section>& getSections()
{
    return sections;
}

const QList<Section>& getSortedSections()
{
    return sortedSections;
}

const QStringList& getSectionNames()
{
    return sectionNames;
}
