#ifndef CASE_H
#define CASE_H

#include "omkit_global.h"

#include <QJsonObject>
#include <QString>

class OMKITSHARED_EXPORT Case
{
public:
    Case();

    bool missingData() const;
    static Case fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;

    static QString makeQuestionFileName(QString caseFilePrefix);
    static QString makeAnswerFileName(QString caseFilePrefix);

    QString name;
    QString questionFileName;
    QString answerFileName;
};

#endif // CASE_H
