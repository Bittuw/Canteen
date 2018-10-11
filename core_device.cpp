#include <QDebug>

#include "core_device.h"

Core::Core_Device::Core_Device(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << "create 'Core_Device'" << this;

    QObject::connect(&m_reader, &IronLogic::Z2USB::readNewKey, this, &Core_Device::receiveCard);
}

void Core::Core_Device::moveToThread(QThread* thread) {
    qDebug() << Q_FUNC_INFO << "moving 'Core_Device' to"<< thread << this;

    QObject::connect(thread, &QThread::started, this, &Core_Device::start);
    QObject::connect(thread, &QThread::finished, this, &Core_Device::stop);

    m_reader.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Core::Core_Device::start() {
    qDebug() << Q_FUNC_INFO << "start 'Core_Device'" << this;
}

void Core::Core_Device::setPersonList(QSet<Core::Person> allowed_list) {
    qDebug() << Q_FUNC_INFO << "set new Allowed List" << this;
    m_allowed_list = allowed_list;
}

void Core::Core_Device::receiveCard(IronLogic::Card card) {
    qDebug() << Q_FUNC_INFO <<
                QObject::tr("receive Card, serial: %1, number: %2")
                .arg(card.serial).arg(card.number) << this;
    m_raw_person = card;
    auto pass_number =
            QString("%1").arg(m_raw_person.serial.toInt(), 3, 10, QChar('0')) +
            QString("%1").arg(m_raw_person.number.toInt(), 5, 10, QChar('0'));
    auto person = std::find_if(m_allowed_list.cbegin(), m_allowed_list.cend(),
                 [&pass_number](const Core::Person& person) -> bool {
                     return person.pass_number == pass_number;
                 });

    if(person != m_allowed_list.cend()) {
        m_last_person = *person;
        emit currentPerson(Enums::FirstRes::FOUND, *person);
    } else {
        emit currentPerson(Enums::FirstRes::NOT_FOUND, {});
    }
}

void Core::Core_Device::stop() {
    qDebug() << Q_FUNC_INFO << "stop 'Core_Device'" << this;
}
