#include "fileutil.h"

#include <QStandardPaths>

const QString FileUtil::DEFAULT_DIR = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Gargoyle-Usage-Checker/";
const QString FileUtil::SETTINGS_FILE = "settings.ini";
const QString FileUtil::PROFILES_FILE = "profiles.json";
