#ifndef CASE_H
#define CASE_H

#include "omkit_global.h"

#include <QJsonObject>
#include <QString>
#include <QUuid>

class OMKITSHARED_EXPORT Case
{
public:
    Case();

    static Case createCase();

    bool missingData() const;
    static Case fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;

    static QString makeQuestionFileName(QString caseFilePrefix);
    static QString makeAnswerFileName(QString caseFilePrefix);

    QUuid id;
    QString name;
    QString questionFileName;
    QString answerFileName;
};

#endif // CASE_H
