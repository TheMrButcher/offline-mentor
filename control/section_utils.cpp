#include "section_utils.h"
#include "settings.h"

namespace {
QHash<QUuid, Section> sections;
}

void loadSections()
{
    sections.clear();
    auto sectionList = Section::findAll(Settings::instance().sectionsPath);
    foreach (const auto& section, sectionList)
        sections[section.id] = section;
}

const QHash<QUuid, Section>& getSections()
{
    return sections;
}
