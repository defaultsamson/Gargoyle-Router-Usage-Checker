#include "settings.h"
#include "fileutil.h"

const QVariant Settings::value() {
    return QSETTINGS->value(_key, _defaultValue);
}

void Settings::setValue(const QVariant value) {
    QSETTINGS->setValue(_key, value);
}

bool Settings::hasValue() {
    QVariant val = QSETTINGS->value(_key);
    return !val.isNull() && val.isValid();
}

void Settings::clearValue() {
    // Old method. setValue(QVariant());
    QSETTINGS->remove(_key);
}

QSettings* Settings::QSETTINGS = new QSettings(FileUtil::DEFAULT_DIR + FileUtil::SETTINGS_FILE, QSettings::IniFormat);

Settings Settings::DISPLAY_ABOVE = Settings("display_above_other_windows", true);
Settings Settings::RELOAD_LOCATION = Settings("open_at_prev_location", true);
Settings Settings::DARK_THEME = Settings("dark_theme", true);
Settings Settings::SHOW_QUOTA = Settings("show_quota_bar", true);
Settings Settings::SHOW_GRAPH = Settings("show_graph", true);
Settings Settings::SNAP_THRESHOLD = Settings("snap_threshold", 20);
Settings Settings::UPDATE_SECONDS = Settings("update_seconds", 5);
Settings Settings::ROUTER_IP = Settings("router_ip");

Settings Settings::PREV_GEOMETRY = Settings("prev_geometry");
