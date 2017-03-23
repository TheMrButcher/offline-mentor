#ifndef TRAININGSETTINGS_H
#define TRAININGSETTINGS_H

#include "omkit_global.h"
#include <QString>

class OMKITSHARED_EXPORT TrainingSettings
{
public:
    TrainingSettings(QString path);

    bool read();
    bool write() const;
    QString localDataPath() const;

    QString path;
    QString lastLogin;
    QString sectionsPath;
    QString solutionsPath;
    bool hasRemoteSolutionsDir;
};

#endif // TRAININGSETTINGS_H
