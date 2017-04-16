#include "answer.h"
#include "case.h"

namespace {
const int FINAL_VERSION = 1 << 20;
}

Answer::Answer()
    : version(0)
{}

Answer Answer::createAnswer(const Case& caseValue)
{
    Answer answer;
    answer.caseId = caseValue.id;
    answer.fileName = caseValue.answerFileName;
    answer.version = 0;
    return answer;
}

Answer Answer::fromJson(const QJsonObject& jsonObject)
{
    Answer answer;
    answer.caseId = QUuid(jsonObject["caseId"].toString(""));
    answer.fileName = jsonObject["fileName"].toString("");
    answer.version = jsonObject["version"].toInt(FINAL_VERSION);
    return answer;
}

QJsonObject Answer::toJson() const
{
    QJsonObject result;
    result["caseId"] = caseId.toString();
    result["fileName"] = fileName;
    result["version"] = version;
    return result;
}

bool Answer::isValid() const
{
    return !caseId.isNull() && !fileName.isEmpty();
}

void Answer::markAsFinal()
{
    version = FINAL_VERSION;
}

bool Answer::isFinal() const
{
    return isValid() && version == FINAL_VERSION;
}
