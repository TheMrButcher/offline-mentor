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
    bool isEqual(const Solution& other) const;
    bool merge(const Solution& other);
    Answer& addAnswer(const Case& caseValue);
    Answer answer(const Case& caseValue) const;
    Solution cloneHeader(QString newDirPath) const;
    int finalAnswersNum() const;

    QUuid sectionId;
    QString fileName;
    QString userName;
    QString dirPath;
    QList<Answer> answers;

private:
    int indexOfOldAnswer(const Answer& newAnswer) const;
};

#endif // SOLUTION_H
