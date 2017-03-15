#include "case.h"

Case::Case()
{}

bool Case::missingData() const
{
    return questionFileName.isEmpty() || answerFileName.isEmpty();
}

Case Case::fromJson(const QJsonObject& jsonObject)
{
    Case c;
    c.name = jsonObject["name"].toString("");
    c.questionFileName = jsonObject["questionFileName"].toString("");
    c.answerFileName = jsonObject["answerFileName"].toString("");
    return c;
}

QJsonObject Case::toJson() const
{
    QJsonObject result;
    result["name"] = name;
    result["questionFileName"] = questionFileName;
    result["answerFileName"] = answerFileName;
    return result;
}

QString Case::makeQuestionFileName(QString caseFilePrefix)
{
    return caseFilePrefix + " Вопрос.html";
}

QString Case::makeAnswerFileName(QString caseFilePrefix)
{
    return caseFilePrefix + " Ответ.html";
}
