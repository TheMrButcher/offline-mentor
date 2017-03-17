#ifndef SOLUTION_UTILS_H
#define SOLUTION_UTILS_H

#include <omkit/section.h>
#include <omkit/solution.h>
#include <QDir>
#include <QString>

enum class SolutionPathType {
    Local
};

void loadSolutions();

Solution getSolution(SolutionPathType type, const Section& section);
bool saveSolution(SolutionPathType type, Solution& solution);

#endif // SOLUTION_UTILS_H
