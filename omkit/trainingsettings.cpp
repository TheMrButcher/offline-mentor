#include "trainingsettings.h"
#include "json_utils.h"
#include <QDir>

TrainingSettings::TrainingSettings(QString path)
    : path(path)
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
    return true;
}

bool TrainingSettings::write() const
{
    QJsonObject rootObj;
    rootObj["lastLogin"] = lastLogin;
    rootObj["sectionsPath"] = sectionsPath;
    rootObj["solutionsPath"] = solutionsPath;
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
