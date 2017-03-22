#include "section_utils.h"
#include "settings.h"

namespace {
QHash<QUuid, Section> sections;
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
    }

    qSort(sectionNames);
}

const QHash<QUuid, Section>& getSections()
{
    return sections;
}

const QStringList& getSectionNames()
{
    return sectionNames;
}
