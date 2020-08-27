#include "gargoyleprofile.h"

GargoyleProfile::GargoyleProfile()
{

}

void GargoyleProfile::setUsage(Usage usage)
{
    lastUsage = currentUsage;
    currentUsage = usage;

    usageDelta = lastUsage.current - currentUsage.current;
    timeDelta = lastUsage.time - currentUsage.time;
}

Usage GargoyleProfile::getUsage()
{
    return currentUsage;
}

Usage GargoyleProfile::getLastUsage()
{
    return lastUsage;
}

uintmax_t GargoyleProfile::getUsageDelta() const
{
    return usageDelta;
}

uintmax_t GargoyleProfile::getTimeDelta() const
{
    return timeDelta;
}
