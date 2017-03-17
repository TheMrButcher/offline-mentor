#include "answer.h"
#include "case.h"

Answer::Answer()
{}

Answer Answer::createAnswer(const Case& caseValue)
{
    Answer answer;
    answer.caseId = caseValue.id;
    answer.fileName = caseValue.answerFileName;
    return answer;
}

Answer Answer::fromJson(const QJsonObject& jsonObject)
{
    Answer answer;
    answer.caseId = QUuid(jsonObject["caseId"].toString(""));
    answer.fileName = jsonObject["fileName"].toString("");
    return answer;
}

QJsonObject Answer::toJson() const
{
    QJsonObject result;
    result["caseId"] = caseId.toString();
    result["fileName"] = fileName;
    return result;
}

bool Answer::isValid()
{
    return !caseId.isNull() && !fileName.isEmpty();
}
