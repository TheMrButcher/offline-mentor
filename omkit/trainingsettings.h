#ifndef TRAININGSETTINGS_H
#define TRAININGSETTINGS_H

#include "omkit_global.h"
#include <QString>
#include <QUuid>
#include <QList>

enum class TrainingAnswerType {
    ZipArchive,
    RemoteDir
};

enum class LoginType {
    Login,
    FirstNameAndSurname,
    OnlyFromGroup
};

class OMKITSHARED_EXPORT TrainingSettings
{
public:
    TrainingSettings(QString path);

    bool read();
    bool write() const;
    QString localDataPath() const;
    QString localGroupsPath() const;
    TrainingAnswerType answerType() const;

    QString path;
    QString lastLogin;
    QString lastFirstName;
    QString lastSurname;
    QString sectionsPath;
    QString solutionsPath;
    QString groupsPath;
    LoginType loginType;
    bool areAllGroupsAllowed;
    QList<QUuid> customGroups;

private:
    bool hasRemoteSolutionsDir;
};

#endif // TRAININGSETTINGS_H
