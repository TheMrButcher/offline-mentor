#include "section.h"
#include "json_utils.h"
#include "utils.h"
#include <QFileInfo>
#include <QJsonArray>
#include <QDir>

namespace {
void findAll(QString path, QList<Section>& dst)
{
    QDir dir(path);
    foreach (const auto& entry, dir.entryList(QStringList("*.oms"), QDir::Files)) {
        Section section;
        section.path = dir.absoluteFilePath(entry);
        if (section.open())
            dst.append(section);
    }
    foreach (const auto& entry, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        findAll(dir.absoluteFilePath(entry), dst);
}
} // namespace

Section::Section()
{}

Section Section::createSection(QString path)
{
    Section section;
    section.path = path;
    section.nextIndex = 1;
    section.id = QUuid::createUuid();
    return section;
}

QList<Section> Section::findAll(QString path)
{
    QList<Section> result;
    if (QFileInfo(path).isDir())
        ::findAll(path, result);
    return result;
}

bool Section::isValid() const
{
    return !id.isNull() && !name.isEmpty() && !path.isEmpty();
}

bool Section::remove()
{
    if (!isValid())
        return false;

    QDir sectionDir = dir();
    foreach (const auto& caseValue, cases) {
        if (!caseValue.questionFileName.isEmpty())
            sectionDir.remove(caseValue.questionFileName);
        if (!caseValue.answerFileName.isEmpty())
            sectionDir.remove(caseValue.answerFileName);
    }
    cases.clear();
    if (!totalFileName.isEmpty()) {
        sectionDir.remove(totalFileName);
        totalFileName = "";
    }
    if (!sectionDir.remove(QFileInfo(path).fileName()))
        return false;
    if (isDirEmpty(sectionDir)) {
        QString dirName = sectionDir.dirName();
        sectionDir.cdUp();
        sectionDir.rmdir(dirName);
    }
    return true;

}

bool Section::open()
{
    QJsonObject rootObj;
    if (!readJSON(path, rootObj))
        return false;
    id = QUuid(rootObj["id"].toString(""));
    if (id.isNull())
        return false;
    name = rootObj["name"].toString("");
    if (name.isEmpty())
        return false;
    description = rootObj["description"].toString("");
    nextIndex = rootObj["nextIndex"].toInt(1);
    totalFileName = rootObj["totalFileName"].toString("");

    QJsonArray casesArray = rootObj["cases"].toArray();
    cases.clear();
    foreach (auto caseValue, casesArray)
        cases.append(Case::fromJson(caseValue.toObject()));

    return true;
}

bool Section::save() const
{
    QJsonObject rootObj;
    rootObj["id"] = id.toString();
    rootObj["name"] = name;
    rootObj["description"] = description;
    rootObj["nextIndex"] = nextIndex;
    rootObj["totalFileName"] = totalFileName;

    QJsonArray casesArray;
    foreach (const auto& c, cases)
        casesArray.append(c.toJson());
    rootObj["cases"] = casesArray;

    return writeJSON(path, rootObj);
}

Section Section::saveAs(QString newPath) const
{
    Section newSection = *this;
    newSection.path = newPath;
    QDir srcDir = dir();
    QDir dstDir = newSection.dir();
    foreach (const auto& caseValue, cases) {
        if (!QFile::copy(srcDir.absoluteFilePath(caseValue.questionFileName),
                         dstDir.absoluteFilePath(caseValue.questionFileName)))
            return Section();
        if (!QFile::copy(srcDir.absoluteFilePath(caseValue.answerFileName),
                         dstDir.absoluteFilePath(caseValue.answerFileName)))
            return Section();
        if (!caseValue.questionImage.isEmpty()) {
            if (!QFile::copy(srcDir.absoluteFilePath(caseValue.questionImage.fileName),
                             dstDir.absoluteFilePath(caseValue.questionImage.fileName)))
                return Section();
        }
        if (!caseValue.answerImage.isEmpty()) {
            if (!QFile::copy(srcDir.absoluteFilePath(caseValue.answerImage.fileName),
                             dstDir.absoluteFilePath(caseValue.answerImage.fileName)))
                return Section();
        }
    }

    if (!totalFileName.isEmpty()) {
        if (!QFile::copy(srcDir.absoluteFilePath(totalFileName),
                         dstDir.absoluteFilePath(totalFileName)))
            return Section();
    }

    if (!newSection.save())
        return Section();
    return newSection;
}

QString Section::nextCaseFilePrefix()
{
    QFileInfo fileInfo(path);
    QString baseName = fileInfo.baseName();
    QDir sectionDir = dir();
    for (int i = 0; i < 100; ++i) {
        QString caseFilePrefix = baseName + " Кейс" + QString::number(nextIndex++);
        if (!sectionDir.exists(Case::makeQuestionFileName(caseFilePrefix))
            && !sectionDir.exists(Case::makeAnswerFileName(caseFilePrefix)))
            return caseFilePrefix;
    }
    return QString();
}

QDir Section::dir() const
{
    return QFileInfo(path).dir();
}

void Section::copyHidden(const Section& section)
{
    path = section.path;
    nextIndex = section.nextIndex;
    id = section.id;
    totalFileName = section.totalFileName;
}

QString Section::makeTotalFileName()
{
    return QFileInfo(path).baseName() + " Итоги.html";
}
