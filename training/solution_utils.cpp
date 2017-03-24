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

void loadSolutionsFrom(SolutionPathType type)
{
    QString path = pathByType(type);
    if (path.isEmpty())
        return;

    path = getUserPath(path);
    if (path.isEmpty())
        return;

    auto newSolutions = Solution::findAll(path);
    foreach (const auto& solution, newSolutions) {
        if (solution.userName != userName())
            continue;
        solutions[SolutionKey{ type, solution.sectionId.toString() }] = solution;
    }
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
} // namespace

void loadSolutions()
{
    loadSolutionsFrom(SolutionPathType::Local);
    loadSolutionsFrom(SolutionPathType::Remote);
}

bool hasSolution(SolutionPathType type, const Section& section)
{
    return solutions.contains(SolutionKey{ type, section.id });
}

Solution getSolution(SolutionPathType type, const Section& section)
{
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
    if (solution.dirPath.isEmpty()) {
        if (!setSolutionDir(type, solution))
            return false;
    }
    if (!solution.save())
        return false;
    solutions[SolutionKey{ type, solution.sectionId }] = solution;
    return true;
}
