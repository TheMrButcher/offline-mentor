#ifndef CASE_H
#define CASE_H

#include <QJsonObject>
#include <QString>

class Case
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
