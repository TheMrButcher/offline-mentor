#ifndef SOLUTION_H
#define SOLUTION_H

#include "omkit_global.h"
#include "answer.h"

#include <QString>
#include <QList>
#include <QUuid>
#include <QDir>

class Section;

class OMKITSHARED_EXPORT Solution
{
public:
    Solution();

    static Solution createSolution(const Section& section);
    static QList<Solution> findAll(QString path);

    bool open();
    bool save();
    QDir dir() const;
    bool isValid() const;
    Answer answer(const Case& caseValue);

    QUuid sectionId;
    QString fileName;
    QString userName;
    QString dirPath;
    QList<Answer> answers;
};

#endif // SOLUTION_H
