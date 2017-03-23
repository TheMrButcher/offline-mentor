#include "settings.h"

namespace {
const QString SETTINGS_FILE_NAME = "Settings.json";
}

Settings&Settings::instance()
{
    static Settings settings;
    return settings;
}

Settings::Settings()
    : TrainingSettings(SETTINGS_FILE_NAME)
{}
