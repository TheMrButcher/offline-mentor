#include "solution.h"
#include "section.h"
#include "json_utils.h"
#include "utils.h"
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QSet>

namespace {
void findAll(QString path, QList<Solution>& dst)
{
    QDir dir(path);
    foreach (const auto& entry, dir.entryList(QStringList("*.omsol"), QDir::Files)) {
        Solution solution;
        solution.dirPath = path;
        solution.fileName = entry;
        if (solution.open())
            dst.append(solution);
    }
    foreach (const auto& entry, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        findAll(dir.absoluteFilePath(entry), dst);
}
} // namespace

Solution::Solution()
{}

Solution Solution::createSolution(const Section& section)
{
    Solution solution;
    solution.sectionId = section.id;
    solution.fileName = QFileInfo(section.path).baseName() + ".omsol";
    return solution;
}

QList<Solution> Solution::findAll(QString path)
{
    QList<Solution> result;
    if (QFileInfo(path).isDir())
        ::findAll(path, result);
    return result;
}

bool Solution::open()
{
    QDir dir(dirPath);
    QString path = dir.absoluteFilePath(fileName);
    QJsonObject rootObj;
    if (!readJSON(path, rootObj))
        return false;

    sectionId = QUuid(rootObj["sectionId"].toString(""));
    if (sectionId.isNull())
        return false;

    userName = rootObj["userName"].toString("");
    if (userName.isEmpty())
        return false;

    QJsonArray answersArray = rootObj["answers"].toArray();
    answers.clear();
    foreach (auto answer, answersArray)
        answers.append(Answer::fromJson(answer.toObject()));

    return true;
}

bool Solution::save()
{
    QJsonObject rootObj;
    rootObj["sectionId"] = sectionId.toString();
    rootObj["userName"] = userName;

    QJsonArray answersArray;
    foreach (const auto& answer, answers)
        answersArray.append(answer.toJson());
    rootObj["answers"] = answersArray;

    QDir dir(dirPath);
    QString path = dir.absoluteFilePath(fileName);
    return writeJSON(path, rootObj);
}

bool Solution::moveTo(QString newDirPath)
{
    auto thisDir = dir();
    QDir otherDir(newDirPath);
    if (!otherDir.exists())
        return false;
    if (!QFile::rename(thisDir.absoluteFilePath(fileName),
                       otherDir.absoluteFilePath(fileName)))
        return false;
    foreach (const auto& answer, answers) {
        if (!QFile::rename(thisDir.absoluteFilePath(answer.fileName),
                           otherDir.absoluteFilePath(answer.fileName)))
            return false;
    }
    dirPath = newDirPath;
    return true;
}

QDir Solution::dir() const
{
    return QDir(dirPath);
}

bool Solution::isValid() const
{
    return !sectionId.isNull() && !userName.isEmpty();
}

bool Solution::isEqual(const Solution& other) const
{
    if (answers.size() != other.answers.size())
        return false;
    for (int i = 0; i < other.answers.size(); ++i) {
        const auto& thisAnswer = answers[i];
        const auto& otherAnswer = other.answers[i];
        if (thisAnswer.caseId != otherAnswer.caseId
            || thisAnswer.version != otherAnswer.version)
            return false;
    }
    return true;
}

bool Solution::merge(const Solution& other)
{
    auto thisDir = dir();
    auto otherDir = other.dir();
    foreach (const auto& answer, other.answers) {
        int index = indexOfOldAnswer(answer);
        if (index == -1)
            continue;
        if (!copyWithOverwrite(otherDir.absoluteFilePath(answer.fileName),
                               thisDir.absoluteFilePath(answer.fileName)))
            return false;
        if (index == answers.size()) {
            answers.append(answer);
        } else {
            answers[index] = answer;
        }
    }
    return save();
}

Answer& Solution::addAnswer(const Case& caseValue)
{
    for (int i = 0; i < answers.size(); ++i) {
        auto& answer = answers[i];
        if (answer.caseId == caseValue.id)
            return answer;
    }
    answers.append(Answer::createAnswer(caseValue));
    return answers.back();
}

Answer Solution::answer(const Case& caseValue) const
{
    foreach (const auto& answer, answers)
        if (answer.caseId == caseValue.id)
            return answer;
    return Answer();
}

Solution Solution::cloneHeader(QString newDirPath) const
{
    Solution result;
    result.sectionId = sectionId;
    result.fileName = fileName;
    result.userName = userName;
    result.dirPath = newDirPath;
    return result;
}

int Solution::finalAnswersNum() const
{
    int result = 0;
    foreach (const auto& answer, answers) {
        if (answer.isFinal())
            result++;
    }
    return result;
}

int Solution::indexOfOldAnswer(const Answer& newAnswer) const
{
    for (int i = 0; i < answers.size(); ++i) {
        const auto& answer = answers[i];
        if (answer.caseId == newAnswer.caseId) {
            if (answer.version >= newAnswer.version)
                return -1;
            return i;
        }
    }
    return answers.size();
}
