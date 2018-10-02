#ifndef SIMPLEQUERY_H
#define SIMPLEQUERY_H

#include <QtNetwork/QNetworkAccessManager>
#include <QObject>

namespace IronLogic {
    struct Card;
}

class SimpleQuery : public QObject
{
    Q_OBJECT
public:
    explicit SimpleQuery(QObject *parent = nullptr);
    void send(IronLogic::Card);
signals:

public slots:

private:
    QNetworkAccessManager manager;
};

#endif // SIMPLEQUERY_H
