#include "section_utils.h"
#include "settings.h"
#include <QHash>
#include <QFileInfo>

namespace {
QList<Section> sections;
QHash<QString, Section> pathToSection;
}

void loadSections()
{
    auto& settings = Settings::instance();
    sections.reserve(settings.knownSections.size());
    foreach (const auto& path, settings.knownSections) {
        Section section;
        section.path = path;
        if (section.open()) {
            sections.append(section);
            pathToSection[path] = section;
        }
    }

    settings.knownSections.clear();
    foreach (const auto& section, sections)
        settings.knownSections.append(section.path);
    settings.write();
}

bool isKnownSection(QString path)
{
    return pathToSection.contains(path);
}

void addSection(const Section& section)
{
    if (isKnownSection(section.path)) {
        for (auto it = sections.begin(); it != sections.end(); ++it) {
            if (it->path == section.path) {
                sections.erase(it);
                break;
            }
        }
    }
    sections.prepend(section);
    pathToSection[section.path] = section;

    auto& settings = Settings::instance();
    if (!settings.knownSections.contains(section.path)) {
        settings.knownSections.prepend(section.path);
        settings.write();
    }
}

const QList<Section>& getSections()
{
    return sections;
}

Section getSection(QString path)
{
    if (isKnownSection(path)) {
        return pathToSection[path];
    }
    return Section();
}

void updateSection(const Section& section)
{
    pathToSection[section.path] = section;
    for (auto it = sections.begin(); it != sections.end(); ++it) {
        if (it->id == section.id) {
            *it = section;
            break;
        }
    }
}
