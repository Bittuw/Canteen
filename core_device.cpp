#include "core_device.h"

#include <QSignalBlocker>
#include <QDebug>
#include <QTime>

Core::Core_Device::Core_Device(QObject *parent) :
    QObject(parent),
    m_block_device(m_reader)
{
    qDebug() << Q_FUNC_INFO << "create 'Core_Device'" << this;
    QObject::connect(&m_reader, &IronLogic::Z2USB::readNewKey, this, &Core_Device::receiveCard);
}

void Core::Core_Device::moveToThread(QThread* thread) {
    qDebug() << Q_FUNC_INFO << "moving 'Core_Device' to"<< thread << this;

    QObject::connect(thread, &QThread::started, this, &Core_Device::start);
    QObject::connect(thread, &QThread::finished, this, &Core_Device::stop);
    QObject::connect(&m_block_device_timer, &Utils::MidnightTimer::TimeOut, [this](){
        m_block_device.unblock();
        m_block_device_timer.stop();
    });

    m_block_device_timer.moveToThread(thread);
    m_reader.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Core::Core_Device::start() {
    m_block_device.unblock();
    qDebug() << Q_FUNC_INFO << "start 'Core_Device'" << this;
}

void Core::Core_Device::setPersonList(QSet<Core::Person> allowed_list) {
    qInfo() << Q_FUNC_INFO << "Set new allowed list with size: "<< allowed_list.size() << this;
    m_allowed_list = allowed_list;
}

void Core::Core_Device::receiveCard(IronLogic::Card card) {

    { // Protect from multi read (Test)
        m_block_device.reblock();
        m_block_device_timer.start(3);
    }

    m_raw_person = card;
    auto pass_number =
            QString("%1").arg(m_raw_person.serial.toInt(), 3, 10, QChar('0')) +
            QString("%1").arg(m_raw_person.number.toInt(), 5, 10, QChar('0'));

    qInfo() << Q_FUNC_INFO <<
                QObject::tr("Receive Card, serial: '%1', number: '%2', result_check: '%3'")
                .arg(card.serial).arg(card.number).arg(pass_number) << this;

    auto person = std::find_if(m_allowed_list.cbegin(), m_allowed_list.cend(),
                 [&pass_number](const Core::Person& person) -> bool {
                     return person.pass_number == pass_number;
                 });

    if(person != m_allowed_list.cend()) {
        qInfo() << Q_FUNC_INFO << QObject::tr("Person found. Name: %1, tab_number: %2").arg(person->full_name).arg(person->tab_number) << this;
        m_last_person = *person;
        emit currentPerson(Enums::FirstRes::FOUND, m_last_person);
    } else {
        qInfo() << Q_FUNC_INFO << QObject::tr("Person not found. pass_number: %2").arg(pass_number) << this;
        emit currentPerson(Enums::FirstRes::NOT_FOUND, {});
    }
}

void Core::Core_Device::stop() {
    qDebug() << Q_FUNC_INFO << "stop 'Core_Device'" << this;
}
