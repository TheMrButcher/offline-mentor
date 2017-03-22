#include "solution_utils.h"
#include "settings.h"
#include "section_utils.h"
#include <omkit/utils.h>
#include <QHash>
#include <QFileInfo>
#include <QSet>

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
} // namespace

void loadSolutions()
{
    solutions.clear();
    userNames.clear();

    const auto& settings = Settings::instance();
    QString localSolutionsPath = settings.localSolutionsPath();
    if (localSolutions.isEmpty()) {
        if (localSolutionsPath.isEmpty())
            return;
        auto solutionList = Solution::findAll(settings.localSolutionsPath());
        foreach (const auto& solution, solutionList) {
            SolutionKey key{ solution.userName, solution.sectionId };
            localSolutions[key] = solution;
        }
    }

    if (!settings.solutionsPath.isEmpty()) {
        auto solutionList = Solution::findAll(settings.solutionsPath);
        foreach (const auto& solution, solutionList) {
            if (!solution.isValid())
                continue;

            SolutionKey key{ solution.userName, solution.sectionId };
            Solution localSolution;
            if (localSolutions.contains(key)) {
                const auto& solutionInMap = localSolutions[key];
                if (solutionInMap.isEqual(solution))
                    continue;
                localSolution = solutionInMap;
            } else {
                auto path = makePath(localSolutionsPath, solution);
                if (path.isEmpty())
                    continue;
                localSolution = solution.cloneHeader(path);
            }

            if (!localSolution.merge(solution))
                continue;
            localSolutions[key] = localSolution;
        }
    }

    const auto& sections = getSections();
    QSet<QString> userNameSet;
    for (auto it = localSolutions.cbegin(); it != localSolutions.cend(); ++it) {
        const auto& solution = *it;
        if (!sections.contains(solution.sectionId))
            continue;
        solutions.append(solution);
        userNameSet.insert(solution.userName);
    }

    foreach (const auto& userName, userNameSet)
        userNames.append(userName);
    qSort(userNames);
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
