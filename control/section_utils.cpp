#include "section_utils.h"
#include "settings.h"
#include <omkit/utils.h>
#include <omkit/zip_utils.h>
#include <QTemporaryDir>

namespace {
QHash<QUuid, Section> sections;
QList<Section> sortedSections;
QStringList sectionNames;

void clearSections()
{
    sections.clear();
    sortedSections.clear();
    sectionNames.clear();
}

QStringList saveSections(const QList<Section>& sectionsToSave)
{
    QStringList importedSectionNames;
    auto rootPath = Settings::instance().sectionsPath;
    if (!getDir(rootPath).exists())
        return importedSectionNames;

    foreach (const auto& section, sectionsToSave) {
        QString path = getNewDir(rootPath, QFileInfo(section.path).baseName());
        if (path.isEmpty())
            continue;

        QDir dir(path);
        QString newFilePath = dir.absoluteFilePath(QFileInfo(section.path).fileName());
        auto importedSection = section.saveAs(newFilePath);
        if (!importedSection.isValid())
            continue;

        importedSectionNames.append(importedSection.name);
        if (sections.contains(section.id))
            sections[section.id].remove();
    }

    if (!importedSectionNames.isEmpty())
        loadSections();
    return importedSectionNames;
}
}

void loadSections()
{
    clearSections();

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

QStringList importSectionsFromFolder(QString path)
{
    QStringList importedSectionNames;
    auto newSections = Section::findAll(path);
    if (newSections.empty())
        return importedSectionNames;
    return saveSections(newSections);
}

QStringList importSectionsFromArchive(QString path)
{
    if (!QFileInfo(path).isFile())
        return QStringList();

    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return QStringList();

    if (!extract(path, tempDir.path()))
        return QStringList();

    return importSectionsFromFolder(tempDir.path());
}

bool saveSections()
{
    if (sections.isEmpty())
        return true;
    auto sectionsToSave = sections.values();
    sections.clear();
    return saveSections(sectionsToSave).size() == sectionsToSave.size();
}
