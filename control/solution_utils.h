#ifndef SOLUTION_UTILS_H
#define SOLUTION_UTILS_H

#include <omkit/solution.h>
#include <QList>
#include <QStringList>

void loadSolutions();
const QList<Solution>& getSolutions();
const Solution& getSolution(QString userName, const QUuid& sectionId);
const QStringList& getUserNames();
bool importSolutionsFromArchive(QString path);
const QStringList& getUserNamesWithoutGroup();
void updateUserNamesWithoutGroup();
bool changeSolutionAuthor(QString userName, const QUuid& sectionId,
                          QString newUserName);
bool saveLocalSolutionsToRemoteDir();

#endif // SOLUTION_UTILS_H
