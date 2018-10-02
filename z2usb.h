#ifndef COMPORT_H
#define COMPORT_H

#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QString>
#include <QObject>

#define CARD_MESSAGE 27
#define NO_CARD_MESSAGE 9

namespace IronLogic {

    struct Card {
        QString number;
        QString serial;
        QString data;
    };

    class Z2USB : public QObject
    {
        Q_OBJECT

    public:
        explicit Z2USB(QObject *parent = nullptr);
        bool getStatus();
        void moveToThread(QThread *thread);

    signals:
        void readNewKey(Card);
        void error();

    public slots:
        bool open();
        void close();
        void doWork();

    private slots:
        void handleReadyRead();
        void handleError(QSerialPort::SerialPortError error);

    protected:
        void checkKey();

    private:
        QSerialPort m_serialport;
        QByteArray m_buffer;
        Card m_lastkey;
    };

}
#endif // COMPORT_H
