#include "solution_utils.h"
#include "settings.h"
#include "section_utils.h"
#include "group_utils.h"
#include <omkit/utils.h>
#include <omkit/zip_utils.h>
#include <QHash>
#include <QFileInfo>
#include <QSet>
#include <QTemporaryDir>

namespace {
struct SolutionKey {
    QString userName;
    QUuid sectionId;
};

bool operator==(const SolutionKey& key1, const SolutionKey& key2)
{
    return key1.userName == key2.userName && key1.sectionId == key2.sectionId;
}

uint qHash(const SolutionKey& key, uint seed)
{
    return qHash(key.userName, seed) ^ qHash(key.sectionId, seed);
}

QList<Solution> solutions;
QHash<SolutionKey, Solution> localSolutions;
QStringList userNames;
QStringList userNamesWithoutGroup;

QString getUserPath(QString path, QString userName)
{
    QDir dir(path);
    if (!dir.exists(userName))
        if (!dir.mkdir(userName))
            return QString();
    return dir.absoluteFilePath(userName);
}

QString makePath(QString rootPath, const Solution& solution)
{
    if (rootPath.isEmpty())
        return QString();

    auto path = getUserPath(rootPath, solution.userName);
    if (path.isEmpty())
        return QString();

    path = getNewDir(path, QFileInfo(solution.fileName).baseName());
    if (path.isEmpty())
        return QString();

    return path;
}

void mergeTo(
        const QList<Solution>& srcSolutions,
        QHash<SolutionKey, Solution>& dstSolutions,
        QString dstSolutionsPath)
{
    foreach (const auto& solution, srcSolutions) {
        if (!solution.isValid())
            continue;

        SolutionKey key{ solution.userName, solution.sectionId };
        Solution dstSolution;
        if (dstSolutions.contains(key)) {
            const auto& solutionInMap = dstSolutions[key];
            if (solutionInMap.isEqual(solution))
                continue;
            dstSolution = solutionInMap;
        } else {
            auto path = makePath(dstSolutionsPath, solution);
            if (path.isEmpty())
                continue;
            dstSolution = solution.cloneHeader(path);
        }

        if (!dstSolution.merge(solution))
            continue;
        dstSolutions[key] = dstSolution;
    }

}

void updateLists()
{
    solutions.clear();
    userNames.clear();
    const auto& sections = getSections();
    QSet<QString> userNameSet;
    for (auto it = localSolutions.cbegin(); it != localSolutions.cend(); ++it) {
        const auto& solution = *it;
        if (!sections.contains(solution.sectionId))
            continue;
        solutions.append(solution);
        userNameSet.insert(it.value().userName);
    }
    foreach (const auto& userName, userNameSet)
        userNames.append(userName);
    qSort(userNames);
    updateUserNamesWithoutGroup();
}

void loadTo(QString path, QHash<SolutionKey, Solution>& dstSolutions)
{
    auto solutionList = Solution::findAll(path);
    foreach (const auto& solution, solutionList) {
        SolutionKey key{ solution.userName, solution.sectionId };
        dstSolutions[key] = solution;
    }
}
} // namespace

void loadSolutions()
{
    const auto& settings = Settings::instance();
    QString localSolutionsPath = settings.localSolutionsPath();
    if (localSolutions.isEmpty()) {
        if (localSolutionsPath.isEmpty())
            return;
        loadTo(localSolutionsPath, localSolutions);
    }

    if (!settings.solutionsPath.isEmpty())
        mergeTo(Solution::findAll(settings.solutionsPath), localSolutions, localSolutionsPath);

    updateLists();
}

const QList<Solution>& getSolutions()
{
    return solutions;
}

const Solution& getSolution(QString userName, const QUuid& sectionId)
{
    static const Solution INVALID_SOLUTION;
    SolutionKey key{ userName, sectionId };
    if (localSolutions.contains(key))
        return localSolutions[key];
    return INVALID_SOLUTION;
}

const QStringList& getUserNames()
{
    return userNames;
}

bool importSolutionsFromArchive(QString path)
{
    if (!QFileInfo(path).isFile())
        return false;

    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return false;

    if (!extract(path, tempDir.path()))
        return false;

    auto newSolutions = Solution::findAll(tempDir.path());
    if (newSolutions.empty())
        return false;

    const auto& settings = Settings::instance();
    mergeTo(newSolutions, localSolutions, settings.localSolutionsPath());
    if (!settings.solutionsPath.isEmpty()) {
        QHash<SolutionKey, Solution> remoteSolutions;
        loadTo(settings.solutionsPath, remoteSolutions);
        mergeTo(newSolutions, remoteSolutions, settings.solutionsPath);
    }
    return true;
}

const QStringList& getUserNamesWithoutGroup()
{
    return userNamesWithoutGroup;
}

void updateUserNamesWithoutGroup()
{
    userNamesWithoutGroup.clear();
    for (const auto& userName : userNames) {
        if (getGroupsByUserName(userName).isEmpty())
            userNamesWithoutGroup.append(userName);
    }
}

bool changeSolutionAuthor(QString userName, const QUuid& sectionId, QString newUserName)
{
    SolutionKey key{ userName, sectionId };
    auto itLocalSolution = localSolutions.find(key);
    if (itLocalSolution == localSolutions.end())
        return false;
    if (getSolution(newUserName, sectionId).isValid())
        return false;

    const auto& settings = Settings::instance();
    QString localSolutionsPath = settings.localSolutionsPath();
    auto localSolution = itLocalSolution.value();
    localSolution.userName = newUserName;
    auto newPath = makePath(localSolutionsPath, localSolution);
    if (!localSolution.moveTo(newPath))
        return false;
    if (!localSolution.save())
        return false;

    if (settings.isNetworkSupported()) {
        auto remoteSolutions = Solution::findAll(
                    getUserPath(settings.solutionsPath, userName));
        for (auto& solution : remoteSolutions) {
            if (solution.sectionId == sectionId) {
                solution.userName = newUserName;
                solution.moveTo(makePath(settings.solutionsPath, solution));
                solution.save();
                break;
            }
        }
    }

    localSolutions.erase(itLocalSolution);
    SolutionKey newKey{ newUserName, sectionId };
    localSolutions[newKey] = localSolution;
    updateLists();
    return true;
}
