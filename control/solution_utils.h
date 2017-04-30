#ifndef SOLUTION_UTILS_H
#define SOLUTION_UTILS_H

#include <omkit/solution.h>
#include <QList>
#include <QStringList>

void loadSolutions();
const QList<Solution>& getSolutions();
const Solution& getSolution(QString userName, const QUuid& sectionId);
const QStringList& getUserNames();
bool loadSolutionsFromArchive(QString path);

#endif // SOLUTION_UTILS_H
