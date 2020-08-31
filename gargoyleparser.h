#ifndef GARGOYLEPARSER_H
#define GARGOYLEPARSER_H

#include "gargoyleprofile.h"
#include <QList>
#include <QReadWriteLock>

enum GargoyleLineType
{
    CURRENT_IP,
    QUOTA_LIMIT,
    QUOTA_USED
};

class GargoyleParser
{
    /// Splits the string into the sections surrounded by square brackets
    QVector<QStringRef> squareBracketSections(const QString &string);

    /// Removes any characters other than 0-9, '.', '-', ',', '[', and ']'
    QString cleanString(const QString string);

public:
    uint32_t timeout = 5000;

    GargoyleParser();

    /// Fetches the HTML of the Gargoyle page and parses the information into the profiles, appending new profiles as needed
    bool update(QString url, QMap<uint64_t, GargoyleProfile*> &profiles, QReadWriteLock &profileLock);
};

#endif // GARGOYLEPARSER_H
