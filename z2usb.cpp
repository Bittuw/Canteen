#include <functional>

#include <QtSerialPort/QSerialPortInfo>
#include <QCoreApplication>
#include <QTextCodec>
#include <QRegExp>
#include <QThread>
#include <QDebug>

#include "z2usb.h"

IronLogic::Z2USB::Z2USB(QObject *parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO <<"initializing Z2USB." << this;

    auto ports = QSerialPortInfo::availablePorts();
    auto serialPortInfo = std::find_if(ports.begin(), ports.end(),
                 [](QSerialPortInfo& info) -> bool
    {
            return !QString::compare(info.description(), QString("USB IronLogic RFID Reader [Serial port]"));
    });
    if(serialPortInfo != ports.end()) {
        qDebug() << Q_FUNC_INFO << "port description:" << serialPortInfo->description() << this;
        m_serialport.setPortName(serialPortInfo->portName());
        m_serialport.setBaudRate(QSerialPort::Baud9600);
    }
}

bool IronLogic::Z2USB::getStatus() {
    return m_serialport.isOpen();
}

void IronLogic::Z2USB::moveToThread(QThread *thread) {
    qDebug() << Q_FUNC_INFO << "moving" << this << "to" << thread;

    QObject::connect(thread, &QThread::started, this, &Z2USB::start);
    QObject::connect(thread, &QThread::finished, this, &Z2USB::stop);

    QObject::moveToThread(thread);
}

void IronLogic::Z2USB::handleReadyRead() {

    qDebug() << Q_FUNC_INFO << "get ready signal." << this;

    m_buffer.append(m_serialport.readAll());

    checkKey();

    qDebug() << Q_FUNC_INFO <<  "signal buffer output:" << m_buffer << this;
}

void IronLogic::Z2USB::start() {
    qDebug() << Q_FUNC_INFO << "prepare to start." << this;
    if(open()) {
        qDebug() << Q_FUNC_INFO << "successfull open port." << this;
        connect(&m_serialport, &QSerialPort::readyRead, this, &Z2USB::handleReadyRead);
        connect(&m_serialport, &QSerialPort::errorOccurred, this, &Z2USB::handleError);
    } else {
        qWarning() << Q_FUNC_INFO << "open error." << this;
    }
}

void IronLogic::Z2USB::handleError(QSerialPort::SerialPortError error) {
    Q_UNUSED(error);
    qWarning() << Q_FUNC_INFO << m_serialport.errorString();
    emit Z2USB::error();
}

void IronLogic::Z2USB::checkKey() {

    auto start_index = 0;
    if((start_index = m_buffer.indexOf("Em")) != -1 && m_buffer.size() > CARD_MESSAGE) {
        m_buffer = m_buffer.mid(start_index, m_buffer.size());
    }

    if(m_buffer.size() >= CARD_MESSAGE && m_buffer.at(0) == 'E') {
        m_buffer.truncate(CARD_MESSAGE);
        QString message = m_buffer;

        QRegExp rx_hex("[0-9a-fA-F]{4}");
        QRegExp rx_digit("(\\d+)");

        auto c_data = 0;
        auto c_serial = 0;
        auto c_number = 0;

        c_data = rx_hex.indexIn(message, c_data);
        c_serial = rx_digit.indexIn(message, c_data+4);
        c_number = rx_digit.indexIn(message, c_serial+4);

        m_lastkey = {
            QString::fromLatin1(m_buffer.mid(c_number, 5)),
            QString::fromLatin1(m_buffer.mid(c_serial, 3)),
            QString::fromLatin1(m_buffer.mid(c_data, 4))
        };

        qDebug() << Q_FUNC_INFO << QObject::tr("Number: %1, Serial %2").arg(m_lastkey.number).arg(m_lastkey.serial) << this;
        m_buffer.clear();
        emit readNewKey(m_lastkey);
    }
    else if(m_buffer.size() >= CARD_MESSAGE && m_buffer.at(0) != 'E') {
        m_buffer.clear();
    }
}

bool IronLogic::Z2USB::open() {
    qDebug() << Q_FUNC_INFO << "opening z2usb port." << this;
    return m_serialport.open(QIODevice::ReadOnly);
}

void IronLogic::Z2USB::stop() {
    qDebug() << Q_FUNC_INFO << "closing z2usb port." << this;
    m_serialport.close();
}
