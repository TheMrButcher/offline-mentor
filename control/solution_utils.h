#ifndef SOLUTION_UTILS_H
#define SOLUTION_UTILS_H

#include <omkit/solution.h>
#include <QList>
#include <QStringList>

void loadSolutions();
const QList<Solution>& getSolutions();
const QStringList& getUserNames();

#endif // SOLUTION_UTILS_H
