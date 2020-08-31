#include "gargoyleparser.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include "iputil.h"

GargoyleParser::GargoyleParser()
{

}

bool GargoyleParser::update(QString url, QMap<uint64_t, GargoyleProfile*> &profiles, QReadWriteLock &profileLock)
{
    // Set up network manager
    QNetworkAccessManager manager;

    manager.setTransferTimeout(timeout);

    // Set up request and reply
    QNetworkRequest request(url);
    std::chrono::nanoseconds requestTime = std::chrono::system_clock::now().time_since_epoch();
    QNetworkReply *reply(manager.get(request));

    // Wait for reply
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != 0)
    {
        qDebug("Network error while updating: %u", reply->error());
        return false;
    }

    // Set all profiles to not updated and not the current device
    profileLock.lockForWrite();
    for (GargoyleProfile *profile : profiles)
    {
        profile->setNotUpdated();
        profile->deviceProfile = false;
    }
    profileLock.unlock();

    // For each HTML line, extract the current IP and profile data
    QMap<uint64_t, Usage> rangeUsages;
    uint32_t currentIp = 0;
    while(reply->canReadLine())
    {
        QString line = reply->readLine();

        GargoyleLineType lineType;

        // Identify line type
        if (line.startsWith("var connectedIp", Qt::CaseInsensitive))
            lineType = GargoyleLineType::CURRENT_IP;
        else if (line.startsWith("quotaLimits", Qt::CaseInsensitive))
            lineType = GargoyleLineType::QUOTA_LIMIT;
        else if (line.startsWith("quotaUsed", Qt::CaseInsensitive))
            lineType = GargoyleLineType::QUOTA_USED;
        else
            continue;

        switch (lineType)
        {
        case GargoyleLineType::CURRENT_IP:
        {
            // Expected format: var connectedIp = "XXX.XXX.XXX.XXX";
            // So extract between the first and last quotation mark
            int start = line.indexOf('"') + 1;
            int end = line.lastIndexOf('"');

            currentIp = IPUtil::parseIp(line.midRef(start, end - start));
            break;
        }

        // Process limit or usage, since both lines use the same format
        case GargoyleLineType::QUOTA_LIMIT:
        case GargoyleLineType::QUOTA_USED:
        {
            // Remove extra characters and split into sections between brackets
            QString clean = cleanString(line);
            QVector<QStringRef> sections = squareBracketSections(clean);

            // Expected format: [Profile ID, IP Range, Data]
            if (sections.size() == 3)
            {
                QVector<QStringRef> dataVals = sections[2].split(',');

                // Expected format: [Upload, Download, Combined]
                if (dataVals.size() == 3)
                {
                    QVector<QStringRef> range = sections[1].split('-');

                    uint32_t minIp, maxIp;
                    // If there's only one IP, not a range, make a range of one IP
                    if (range.size() == 1)
                    {
                        minIp = IPUtil::parseIp(range[0]);
                        maxIp = minIp;
                    }
                    // Parse the IP range into min and max IPs
                    else if (range.size() == 2)
                    {
                        minIp = IPUtil::parseIp(range[0]);
                        maxIp = IPUtil::parseIp(range[1]);
                    }
                    // Otherwise if there's an unexpected number of IPs, give up
                    else
                    {
                        continue;
                    }

                    // Create a range key for the map
                    uint64_t rangeKey = IPUtil::createIpRange(minIp, maxIp);
                    // Get the download usage
                    uint64_t downloadData = dataVals[1].toULongLong();

                    // Add data to existing usage
                    if (rangeUsages.contains(rangeKey))
                    {
                        switch (lineType)
                        {
                        case GargoyleLineType::QUOTA_LIMIT:
                        {
                            rangeUsages[rangeKey].max = downloadData;
                            break;
                        }

                        case GargoyleLineType::QUOTA_USED:
                        {
                            rangeUsages[rangeKey].current = downloadData;
                            break;
                        }

                        default:
                            continue;
                        }
                    }
                    // Create new usage
                    else
                    {
                        Usage rangeUsage{requestTime, 0, 0};

                        switch (lineType)
                        {
                        case GargoyleLineType::QUOTA_LIMIT:
                        {
                            rangeUsage.max = downloadData;
                            break;
                        }

                        case GargoyleLineType::QUOTA_USED:
                        {
                            rangeUsage.current = downloadData;
                            break;
                        }

                        default:
                            continue;
                        }

                        rangeUsages[rangeKey] = rangeUsage;
                    }
                }
            }
            break;
        }

        default:
            continue;
        }
    }

    profileLock.lockForWrite();
    // Add usages to profiles, creating profiles as needed
    for (uint64_t rangeKey : rangeUsages.keys())
    {
        if (profiles.contains(rangeKey))
        {
            profiles[rangeKey]->setUsage(rangeUsages[rangeKey]);
        }
        else
        {
            GargoyleProfile *profile = new GargoyleProfile(rangeKey);

            QString ipRange = IPUtil::ipRangeToString(rangeKey);
            profile->name = ipRange;
            profile->displayIpRange = ipRange;

            profile->setUsage(rangeUsages[rangeKey]);

            profiles[rangeKey] = profile;
        }
    }

    // Set the device profile
    for (GargoyleProfile *profile : profiles)
    {
        if (profile->containsIp(currentIp))
            profile->deviceProfile = true;
    }
    profileLock.unlock();

    return true;
}

QVector<QStringRef> GargoyleParser::squareBracketSections(const QString &string)
{
    QVector<QStringRef> sections;

    int bracketStart = -1;
    for(int i = 0; i < string.length(); i++)
    {
        if (string[i] == '[')
        {
            bracketStart = i + 1;
        }
        else if (string[i] == ']' && bracketStart >= 0)
        {
            sections.append(string.midRef(bracketStart, i - bracketStart));
            bracketStart = -1;
        }
    }

    return sections;
}

QString GargoyleParser::cleanString(const QString string)
{
    QString cleaned;

    for (QChar character : string)
    {
        if (character.isDigit() || character == '.' || character == '-' || character == ',' || character == '[' || character == ']')
            cleaned.append(character);
    }

    return cleaned;
}
