#include "simplequery.h"
#include "z2usb.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

SimpleQuery::SimpleQuery(QObject *parent) : QObject(parent)
{

}

void SimpleQuery::send(IronLogic::Card card) {
    //QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    QNetworkRequest request;
//    request.setSslConfiguration(config);


    QUrl url;
     url.setUrl(QObject::tr("http://lgprk.ru:80/api/v1/food/?key=d412423r4sdrfsdfdsfsf&serial=%1&number=%2&place=1")
                .arg(card.serial)
                .arg(card.number)
                );

   request.setUrl(url);
   manager.get(request);
}
