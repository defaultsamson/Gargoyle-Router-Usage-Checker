#ifndef GARGOYLEPARSER_H
#define GARGOYLEPARSER_H

#include "gargoyleprofile.h"
#include <QList>

class GargoyleParser
{
public:
    GargoyleParser();

    /// Fetches the HTML of the Gargoyle page and parses the information into the profiles, appending new profiles as needed
    void update(QString url, QList<GargoyleProfile> profiles);
};

#endif // GARGOYLEPARSER_H
