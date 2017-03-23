#ifndef SETTINGS_H
#define SETTINGS_H

#include <omkit/trainingsettings.h>

class Settings : public TrainingSettings
{
public:
    static Settings& instance();

private:
    Settings();
};

#endif // SETTINGS_H
