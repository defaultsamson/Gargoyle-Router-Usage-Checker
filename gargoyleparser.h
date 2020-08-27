#ifndef GARGOYLEPARSER_H
#define GARGOYLEPARSER_H

#include "gargoyleprofile.h"
#include <QList>



class GargoyleParser
{
public:
    GargoyleParser();

    void update(QList<GargoyleProfile>);
};

#endif // GARGOYLEPARSER_H
