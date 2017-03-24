#ifndef SOLUTION_UTILS_H
#define SOLUTION_UTILS_H

#include <omkit/section.h>
#include <omkit/solution.h>
#include <QDir>
#include <QString>

enum class SolutionPathType {
    Local,
    Remote
};

void loadSolutions();

bool hasSolution(SolutionPathType type, const Section& section);
Solution getSolution(SolutionPathType type, const Section& section);
const Solution& peekSolution(SolutionPathType type, const Section& section);
bool saveSolution(SolutionPathType type, Solution& solution);

#endif // SOLUTION_UTILS_H
