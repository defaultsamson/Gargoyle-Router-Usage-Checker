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
};

#endif // GARGOYLEPARSER_H
