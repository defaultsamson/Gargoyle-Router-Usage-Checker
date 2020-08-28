#include "gargoyleparser.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

GargoyleParser::GargoyleParser()
{

}

/// Splits the string into the sections surrounded by square brackets
QStringList squareBracketSections(QString string)
{
    QStringList sections;

    int bracketStart = -1;
    for(int i = 0; i < string.length(); i++)
    {
        if (string[i] == '[')
        {
            bracketStart = i + 1;
        }
        else if (string[i] == ']' && bracketStart >= 0)
        {
            sections.append(string.mid(bracketStart, i - bracketStart));
            bracketStart = -1;
        }
    }

    return sections;
}

/// Removes any characters other than 0-9, '.', '-', ',', '[', and ']'
QString cleanString(QString string)
{
    QString cleaned;

    foreach (QChar character, string)
    {
        if (character.isDigit() || character == '.' || character == '-' || character == ',' || character == '[' || character == ']')
            cleaned.append(character);
    }

    return cleaned;
}

bool GargoyleParser::update(QString url, QList<GargoyleProfile> profiles)
{
    // Set up network manager
    QNetworkAccessManager manager;
    manager.setTransferTimeout(timeout);

    // Set up request and reply
    QNetworkRequest request(url);
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

    // Set all profiles to not updated
    foreach (GargoyleProfile profile, profiles)
    {
        profile.updated = false;
    }

    // For each HTML line, extract the current IP and profile data
    QString currentIp;
    while(reply->canReadLine())
    {
        QString line = reply->readLine();

        // Quota info
        if (line.startsWith("quotaUsed", Qt::CaseInsensitive))
        {
            // Remove extra characters and split into sections between brackets
            QStringList sections = squareBracketSections(cleanString(line));

            if (sections.size() == 3)
            {
                QString range = sections[1];
                QString usage = sections[2];

                qDebug() << "Range" << range << "with usage" << usage;
            }
        }
        else if (line.startsWith("quotaLimits", Qt::CaseInsensitive))
        {
            // Remove extra characters and split into sections between brackets
            QStringList sections = squareBracketSections(cleanString(line));

            if (sections.size() == 3)
            {
                QString range = sections[1];
                QString limit = sections[2];

                qDebug() << "Range" << range << "with limit" << limit;
            }
        }
        // Get current device IP
        else if (line.startsWith("var connectedIp", Qt::CaseInsensitive))
        {
            int start = line.indexOf('"') + 1;
            int end = line.lastIndexOf('"');

            currentIp = line.mid(start, end - start);

            qDebug("Your IP: %s", qUtf8Printable(currentIp));
        }
    }

    return true;
}
