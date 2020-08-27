#ifndef GARGOYLEPROFILE_H
#define GARGOYLEPROFILE_H

#include <cstdint>

struct Usage
{
    uintmax_t time, current, max;
};

class GargoyleProfile
{
public:
    // Whether this profile was updated last time it was fetched
    bool updated;

    GargoyleProfile();

    /// Sets the new usage and calculates the usage speed
    void setUsage(Usage usage);

    /// Returns the current usage
    Usage getUsage();
    /// Returns the last usage
    Usage getLastUsage();

    uintmax_t getUsageDelta() const;
    uintmax_t getTimeDelta() const;

private:
    Usage currentUsage, lastUsage;
    uintmax_t usageDelta, timeDelta;
};

#endif // GARGOYLEPROFILE_H
