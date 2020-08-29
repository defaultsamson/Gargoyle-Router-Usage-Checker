#ifndef GARGOYLEPARSER_H
#define GARGOYLEPARSER_H

#include "gargoyleprofile.h"
#include <QList>

enum GargoyleLineType
{
    NONE,
    CURRENT_IP,
    QUOTA_LIMITS,
    QUOTA_USED
};

class GargoyleParser
{
    /// Splits the string into the sections surrounded by square brackets
    QVector<QStringRef> squareBracketSections(QString string);

    /// Removes any characters other than 0-9, '.', '-', ',', '[', and ']'
    QString cleanString(QString string);

    /// Parses a string IP representation to an integer
    uint32_t parseIp(QStringRef ip);

    /// Creates an IP range in a 64 bit integer rather than two 32 bit integers
    uint64_t createIpRange(uint32_t minIp, uint32_t maxIp);

    /// Creates a string representation of an IP range
    QString ipRangeToString(uint32_t minIp, uint32_t maxIp);

public:
    uint32_t timeout = 5000;

    GargoyleParser();

    /// Fetches the HTML of the Gargoyle page and parses the information into the profiles, appending new profiles as needed
    bool update(QString url, QList<GargoyleProfile> profiles);
};

#endif // GARGOYLEPARSER_H
