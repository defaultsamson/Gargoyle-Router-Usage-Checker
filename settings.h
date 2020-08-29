#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QStandardPaths>

class Settings
{
public:
    static const QString DEFAULT_DIR;
    static const QString SETTINGS_FILE;

    static QSettings* QSETTINGS;

    static Settings DISPLAY_ABOVE;
    static Settings RELOAD_LOCATION;
    static Settings DARK_THEME;
    static Settings SHOW_QUOTA;
    static Settings SNAP_THRESHOLD;
    static Settings SHOW_GRAPH;

    static Settings UPDATE_SECONDS;
    static Settings ROUTER_IP;

    static Settings PREV_GEOMETRY;

private:
    Settings(const QString key, const QVariant defaultValue = QVariant())
        : _key(key), _defaultValue(defaultValue) {}

    const QString _key;
    const QVariant _defaultValue;

public:
    const QVariant value();
    void setValue(const QVariant value);
    bool hasValue();
    void clearValue();
};

#endif // SETTINGS_H
