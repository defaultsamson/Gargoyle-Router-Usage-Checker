#include "gargoyleparser.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <regex>

GargoyleParser::GargoyleParser()
{

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

    QString html = reply->readAll();
    qDebug(qUtf8Printable(html));

    return true;
}
