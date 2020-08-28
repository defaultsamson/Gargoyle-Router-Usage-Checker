#ifndef GARGOYLEPROFILE_H
#define GARGOYLEPROFILE_H

#include <QString>
#include <cstdint>

struct Usage
{
    uintmax_t time, current, max;
};

class GargoyleProfile
{
public:
    /// Whether this profile was updated last time it was fetched
    bool updated;

    /// Whether this profile is the current device's profile
    bool deviceProfile;

    /// Profile name for display
    QString name;

    /// Shortcut for single IP
    GargoyleProfile(uint32_t ip, Usage usage);
    GargoyleProfile(uint32_t minIp, uint32_t maxIp, Usage usage);

    bool operator==(GargoyleProfile profile) const noexcept;
    bool equals(uint32_t minIp, uint32_t maxIp) const;

    bool containsIp(uint32_t ip) const;

    /// Sets the new usage and calculates the usage speed
    void setUsage(Usage usage);

    /// Returns the current usage
    Usage getUsage() const;
    /// Returns the last usage
    Usage getLastUsage() const;

    intmax_t getUsageDelta() const;
    intmax_t getTimeDelta() const;

private:
    /// The profile minimum and maximum IPs
    uint32_t minIp, maxIp;

    Usage currentUsage, lastUsage;
    intmax_t usageDelta, timeDelta;
};

#endif // GARGOYLEPROFILE_H
