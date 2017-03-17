#ifndef ANSWER_H
#define ANSWER_H

#include "omkit_global.h"

#include <QJsonObject>
#include <QString>
#include <QUuid>

class Case;

class OMKITSHARED_EXPORT Answer
{
public:
    Answer();

    static Answer createAnswer(const Case& caseValue);

    static Answer fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;
    bool isValid();

    QUuid caseId;
    QString fileName;
};

#endif // ANSWER_H
