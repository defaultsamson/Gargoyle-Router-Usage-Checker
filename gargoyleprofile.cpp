#include "gargoyleprofile.h"
#include "IPUtil.h"

GargoyleProfile::GargoyleProfile(uint32_t ip) : GargoyleProfile(ip, ip)
{
}

GargoyleProfile::GargoyleProfile(uint64_t ipRange) : GargoyleProfile(IPUtil::rangeStart(ipRange), IPUtil::rangeEnd(ipRange))
{
}

GargoyleProfile::GargoyleProfile(uint32_t minIp, uint32_t maxIp)
{
    this->minIp = minIp;
    this->maxIp = maxIp;
}

bool GargoyleProfile::operator==(const GargoyleProfile &profile) const noexcept
{
    return this->minIp == profile.minIp && this->maxIp == profile.maxIp;
}

bool GargoyleProfile::operator==(uint64_t ipRange) const noexcept
{
    return IPUtil::createIpRange(minIp, maxIp) == ipRange;
}

bool GargoyleProfile::equals(uint32_t minIp, uint32_t maxIp) const
{
    return this->minIp == minIp && this->maxIp == maxIp;
}

bool GargoyleProfile::containsIp(uint32_t ip) const
{
    return ip >= minIp && ip <= maxIp;
}

uint32_t GargoyleProfile::getMinIp() const
{
    return minIp;
}

uint32_t GargoyleProfile::getMaxIp() const
{
    return maxIp;
}

bool GargoyleProfile::isUpdated() const
{
    return updated;
}

void GargoyleProfile::setNotUpdated()
{
    updated = false;
}

void GargoyleProfile::setUsage(Usage usage)
{
    // If there isn't an old usage, just pass the current one as the old
    lastUsage = hasCurrentUsage ? currentUsage : usage;
    currentUsage = usage;
    hasCurrentUsage = true;

    usageDelta = currentUsage.current - lastUsage.current;
    timeDelta = currentUsage.time - lastUsage.time;

    updated = true;
}

Usage GargoyleProfile::getUsage() const
{
    assert(updated);
    return currentUsage;
}

Usage GargoyleProfile::getLastUsage() const
{
    assert(updated);
    return lastUsage;
}

int64_t GargoyleProfile::getUsageDelta() const
{
    assert(updated);
    return usageDelta;
}

std::chrono::nanoseconds GargoyleProfile::getTimeDelta() const
{
    assert(updated);
    return timeDelta;
}
