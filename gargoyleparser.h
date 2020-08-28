#ifndef GARGOYLEPARSER_H
#define GARGOYLEPARSER_H

#include "gargoyleprofile.h"
#include <QList>

class GargoyleParser
{
public:
    unsigned int timeout = 5000;

    GargoyleParser();

    /// Fetches the HTML of the Gargoyle page and parses the information into the profiles, appending new profiles as needed
    bool update(QString url, QList<GargoyleProfile> profiles);

private:
    /// Splits the string into the sections surrounded by square brackets
    QVector<QStringRef> squareBracketSections(QString string);

    /// Removes any characters other than 0-9, '.', '-', ',', '[', and ']'
    QString cleanString(QString string);

    /// Parses a string IP representation to an integer
    uint32_t parseIp(QStringRef ip);
};

#endif // GARGOYLEPARSER_H
