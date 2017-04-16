#include "solution_utils.h"
#include "user_utils.h"
#include "settings.h"
#include <omkit/utils.h>
#include <QFileInfo>
#include <QHash>

namespace {
struct SolutionKey {
    SolutionPathType type;
    QUuid sectionId;
};

bool operator==(const SolutionKey& key1, const SolutionKey& key2)
{
    return key1.type == key2.type && key1.sectionId == key2.sectionId;
}

uint qHash(const SolutionKey& key, uint seed)
{
    return qHash(key.sectionId, seed);
}

QString pathByType(SolutionPathType type)
{
    const auto& settings = Settings::instance();
    switch (type) {
    case SolutionPathType::Local: return settings.localDataPath();
    case SolutionPathType::Remote:
        if (QFileInfo(settings.solutionsPath).isDir())
            return settings.solutionsPath;
        return QString();
    }
    return QString();
}

QHash<SolutionKey, Solution> solutions;
bool isSynced = false;

bool loadSolutionsFrom(SolutionPathType type)
{
    QString path = pathByType(type);
    if (path.isEmpty())
        return false;

    path = getUserPath(path);
    if (path.isEmpty())
        return false;

    auto newSolutions = Solution::findAll(path);
    foreach (const auto& solution, newSolutions) {
        if (solution.userName != userName())
            continue;
        solutions[SolutionKey{ type, solution.sectionId.toString() }] = solution;
    }
    return true;
}

bool setSolutionDir(SolutionPathType type, Solution& solution)
{
    QString path = pathByType(type);
    if (path.isEmpty())
        return false;

    path = getUserPath(path);
    if (path.isEmpty())
        return false;

    path = getNewDir(path, QFileInfo(solution.fileName).baseName());
    if (path.isEmpty())
        return false;

    solution.dirPath = path;
    return true;
}

void sync(SolutionPathType from, SolutionPathType to)
{
    auto solutionsCopy = solutions;
    for (auto it = solutionsCopy.begin(); it != solutionsCopy.end(); ++it) {
        const auto& srcKey = it.key();
        if (srcKey.type != from)
            continue;
        const auto& srcSolution = it.value();
        SolutionKey dstKey{ to, srcKey.sectionId };
        Solution dstSolution;
        if (solutions.contains(dstKey)) {
            const auto& solutionInMap = solutions[dstKey];
            if (solutionInMap.isEqual(srcSolution))
                continue;
            dstSolution = solutionInMap;
        } else {
            dstSolution = srcSolution.cloneHeader("");
            if (!setSolutionDir(to, dstSolution))
                continue;
        }

        if (!dstSolution.merge(srcSolution))
            continue;
        solutions[dstKey] = dstSolution;
    }
}

void syncWithRemote()
{
    isSynced = loadSolutionsFrom(SolutionPathType::Remote);
    sync(SolutionPathType::Remote, SolutionPathType::Local);
    sync(SolutionPathType::Local, SolutionPathType::Remote);
}
} // namespace

void loadSolutions()
{
    loadSolutionsFrom(SolutionPathType::Local);
    syncWithRemote();
}

bool hasSolution(SolutionPathType type, const Section& section)
{
    if (type == SolutionPathType::Remote && !isSynced)
        return false;
    return solutions.contains(SolutionKey{ type, section.id });
}

Solution getSolution(SolutionPathType type, const Section& section)
{
    if (type == SolutionPathType::Remote && !isSynced)
        return Solution();

    SolutionKey key{ type, section.id };
    if (!solutions.contains(key)) {
        Solution solution = Solution::createSolution(section);
        solution.userName = userName();
        if (!saveSolution(type, solution)) {
            return Solution();
        }
        return solution;
    }
    return solutions[key];
}

const Solution& peekSolution(SolutionPathType type, const Section& section)
{
    return solutions[SolutionKey{ type, section.id }];
}

bool saveSolution(SolutionPathType type, Solution& solution)
{
    if (type == SolutionPathType::Remote && !isSynced)
        return false;
    if (solution.dirPath.isEmpty()) {
        if (!setSolutionDir(type, solution))
            return false;
    }
    if (!solution.save())
        return false;
    solutions[SolutionKey{ type, solution.sectionId }] = solution;
    return true;
}

bool mergeSolution(const Solution& srcSolution,
                   SolutionPathType dstType, Solution& dstSolution)
{
    if (dstType == SolutionPathType::Remote && !isSynced)
        return false;
    if (dstSolution.dirPath.isEmpty()) {
        if (!saveSolution(dstType, dstSolution))
            return false;
    }
    if (!dstSolution.merge(srcSolution))
        return false;
    solutions[SolutionKey{ dstType, dstSolution.sectionId }] = dstSolution;
    return true;
}
