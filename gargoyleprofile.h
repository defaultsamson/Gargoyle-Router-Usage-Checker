#ifndef GARGOYLEPROFILE_H
#define GARGOYLEPROFILE_H

#include <QString>
#include <chrono>

struct Usage
{
    uint32_t minIp, maxIp;
    std::chrono::nanoseconds time;
    uint64_t current, max;
};

class GargoyleProfile
{
public:
    /// Whether this profile was updated last time it was fetched
    bool updated = true;

    /// Whether this profile is the current device's profile
    bool deviceProfile = false;

    /// Profile name for display
    QString name;

    /// IP range for display
    QString displayIpRange;

    /// Whether this ip range is updating on the graph
    bool showInGraph = true;

    GargoyleProfile(Usage usage);

    bool operator==(GargoyleProfile profile) const noexcept;
    bool equals(uint32_t minIp, uint32_t maxIp) const;

    bool containsIp(uint32_t ip) const;

    /// Sets the new usage and calculates the usage speed
    void setUsage(Usage usage);

    /// Returns the current usage
    Usage getUsage() const;
    /// Returns the last usage
    Usage getLastUsage() const;

    int64_t getUsageDelta() const;
    std::chrono::nanoseconds getTimeDelta() const;

    uint32_t getMinIp() const;
    uint32_t getMaxIp() const;

private:
    /// The profile minimum and maximum IPs
    uint32_t minIp, maxIp;

    Usage currentUsage, lastUsage;

    int64_t usageDelta;
    std::chrono::nanoseconds timeDelta;
};

#endif // GARGOYLEPROFILE_H
