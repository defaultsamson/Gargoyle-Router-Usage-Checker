#ifndef IPUTIL_H
#define IPUTIL_H

#include <stdint.h>
#include <QStringRef>
#include <QVector>

class IPUtil {
public:
    /// Creates an IP range in a 64 bit integer rather than two 32 bit integers
    static uint64_t createIpRange(uint32_t minIp, uint32_t maxIp)
    {
        uint64_t ipRange = maxIp;
        ipRange = (ipRange << 32) | minIp;

        return ipRange;
    }

    /// Parses a string IP representation to an integer
    static uint32_t parseIp(const QStringRef ip)
    {
        uint32_t intIp = 0;

        for (QStringRef subnet : ip.split('.'))
        {
            // Shift by 8 bits for each byte
            intIp <<= 8;

            // Add this level of subnet
            intIp |= subnet.toInt();
        }

        return intIp;
    }

    /// Creates a string representation of an IP range
    static QString ipRangeToString(uint64_t ipRange)
    {
        return ipRangeToString((ipRange >> 32) & 0xFFFFFF, ipRange & 0xFFFFFF);
    }

    /// Creates a string representation of an IP range
    static QString ipRangeToString(uint32_t minIp, uint32_t maxIp)
    {
        return ipToString(minIp) + " - " + ipToString(maxIp);
    }

    /// Creates a string representation of an IP
    static QString ipToString(uint32_t ip)
    {
        return QString::asprintf("%d.%d.%d.%d",
                                 (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    }
};

#endif // IPUTIL_H
