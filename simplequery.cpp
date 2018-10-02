#include "simplequery.h"
#include "z2usb.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

SimpleQuery::SimpleQuery(QObject *parent) : QObject(parent)
{

}

void SimpleQuery::send(IronLogic::Card card) {
    QUrl url(
                QObject::tr("http://lgprk.ru/api/v1/food/?key=d412423r4sdrfsdfdsfsf&serial=%1&number=%2&place=1")
                .arg(card.serial)
                .arg(card.number)
   );
   manager.get(QNetworkRequest(url));
}
