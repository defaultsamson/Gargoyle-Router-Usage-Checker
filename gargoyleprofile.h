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

    ///

    /// Profile name for display
    QString name;

    /// Shortcut for single IP
    GargoyleProfile(int ip, Usage usage);
    GargoyleProfile(int minIp, int maxIp, Usage usage);

    bool operator==(GargoyleProfile profile) const noexcept;
    bool equals(int minIp, int maxIp) const;

    bool containsIp(int ip) const;

    /// Sets the new usage and calculates the usage speed
    void setUsage(Usage usage);

    /// Returns the current usage
    Usage getUsage() const;
    /// Returns the last usage
    Usage getLastUsage() const;

    uintmax_t getUsageDelta() const;
    uintmax_t getTimeDelta() const;

private:
    /// The profile minimum and maximum IPs
    int minIp, maxIp;

    Usage currentUsage, lastUsage;
    uintmax_t usageDelta, timeDelta;
};

#endif // GARGOYLEPROFILE_H
