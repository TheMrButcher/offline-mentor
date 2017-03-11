#include "section.h"
#include "settings.h"

#include <QDir>

Section::Section()
{}

QList<Section> Section::loadSections()
{
    const auto& settings = Settings::instance();
    QDir sectionsDir(settings.sectionsPath);

    QList<Section> result;
    if (!sectionsDir.exists())
        return result;

    auto entries = sectionsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const auto& entry, entries) {
        auto name = entry.fileName();

        Section section;
        section.name = name;
        section.directoryName = name;
        result.append(section);
    }

    return result;
}
