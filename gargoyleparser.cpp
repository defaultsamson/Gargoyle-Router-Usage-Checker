#include "gargoyleparser.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

GargoyleParser::GargoyleParser()
{

}

void GargoyleParser::update(QString url, QList<GargoyleProfile> profiles)
{
    QUrl url = QUrl("http://192.168.1.1");
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply(manager.get(request));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    qDebug(reply->readAll());
}
