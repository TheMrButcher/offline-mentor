#ifndef TRAININGSETTINGS_H
#define TRAININGSETTINGS_H

#include "omkit_global.h"
#include <QString>

enum class TrainingAnswerType {
    ZipArchive,
    RemoteDir
};

class OMKITSHARED_EXPORT TrainingSettings
{
public:
    TrainingSettings(QString path);

    bool read();
    bool write() const;
    QString localDataPath() const;
    TrainingAnswerType answerType() const;

    QString path;
    QString lastLogin;
    QString sectionsPath;
    QString solutionsPath;

private:
    bool hasRemoteSolutionsDir;
};

#endif // TRAININGSETTINGS_H
