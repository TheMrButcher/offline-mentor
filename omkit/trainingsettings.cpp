#include "trainingsettings.h"
#include "json_utils.h"
#include "smallbimap.h"
#include <QDir>
#include <QJsonArray>

namespace {
const SmallBimap<LoginType, QString> LOGIN_TYPE_NAMES(
        { LoginType::Login, LoginType::FirstNameAndSurname, LoginType::OnlyFromGroup },
        { "Login", "FirstNameAndSurname", "OnlyFromGroup" });
} // namespace

TrainingSettings::TrainingSettings(QString path)
    : path(path)
    , loginType(LoginType::FirstNameAndSurname)
    , areAllGroupsAllowed(true)
    , hasRemoteSolutionsDir(false)
{}

bool TrainingSettings::read()
{
    QJsonObject rootObj;
    if (!readJSON(path, rootObj))
        return false;
    lastLogin = rootObj["lastLogin"].toString(lastLogin);
    sectionsPath = rootObj["sectionsPath"].toString(sectionsPath);
    solutionsPath = rootObj["solutionsPath"].toString(solutionsPath);
    hasRemoteSolutionsDir =
            !solutionsPath.isEmpty() && QFileInfo(solutionsPath).isDir();
    groupsPath = rootObj["groupsPath"].toString(groupsPath);
    loginType = LOGIN_TYPE_NAMES.valueBySecondOr(
                rootObj["loginType"].toString(), LoginType::FirstNameAndSurname);
    areAllGroupsAllowed = rootObj["areAllGroupsAllowed"].toBool(areAllGroupsAllowed);

    customGroups.clear();
    QJsonArray customGroupsJSON = rootObj["customGroups"].toArray();
    for (const QJsonValue& json : customGroupsJSON) {
        QUuid id(json.toString());
        if (!id.isNull())
            customGroups.append(id);
    }

    return true;
}

bool TrainingSettings::write() const
{
    QJsonObject rootObj;
    rootObj["lastLogin"] = lastLogin;
    rootObj["sectionsPath"] = sectionsPath;
    rootObj["solutionsPath"] = solutionsPath;
    rootObj["groupsPath"] = groupsPath;
    rootObj["areAllGroupsAllowed"] = areAllGroupsAllowed;
    rootObj["loginType"] = LOGIN_TYPE_NAMES.valueByFirst(loginType);

    QJsonArray customGroupsJSON;
    for (const auto& id : customGroups)
        customGroupsJSON.append(id.toString());
    rootObj["customGroups"] = customGroupsJSON;

    return writeJSON(path, rootObj);
}

QString TrainingSettings::localDataPath() const
{
    QDir dir;
    if (!dir.exists("localData"))
        if (!dir.mkdir("localData"))
            return QString();
    return dir.absoluteFilePath("localData");
}

TrainingAnswerType TrainingSettings::answerType() const
{
    return hasRemoteSolutionsDir
            ? TrainingAnswerType::RemoteDir
            : TrainingAnswerType::ZipArchive;
}
