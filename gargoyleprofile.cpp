#include "gargoyleprofile.h"

GargoyleProfile::GargoyleProfile(int ip, Usage usage) : GargoyleProfile(ip, ip, usage)
{
}

GargoyleProfile::GargoyleProfile(int minIp, int maxIp, Usage usage)
{
    // Set up the IP range
    this->minIp = minIp;
    this->maxIp = maxIp;

    // Set up the usage for the profile
    currentUsage = usage;
    setUsage(usage);
}

bool GargoyleProfile::operator==(GargoyleProfile profile) const noexcept
{
    return this->minIp == profile.minIp && this->maxIp == profile.maxIp;
}

bool GargoyleProfile::equals(int minIp, int maxIp) const
{
    return this->minIp == minIp && this->maxIp == maxIp;
}

bool GargoyleProfile::containsIp(int ip) const
{
    return ip >= minIp && ip <= maxIp;
}

void GargoyleProfile::setUsage(Usage usage)
{
    lastUsage = currentUsage;
    currentUsage = usage;

    usageDelta = lastUsage.current - currentUsage.current;
    timeDelta = lastUsage.time - currentUsage.time;
}

Usage GargoyleProfile::getUsage() const
{
    return currentUsage;
}

Usage GargoyleProfile::getLastUsage() const
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
