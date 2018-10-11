#ifndef CORE_DEVICE_H
#define CORE_DEVICE_H

#include <QObject>
#include <QThread>
#include <QSet>

#include "virtual_core.h"
#include "person.h"
#include "z2usb.h"


namespace Core {
    class Core_Device : public QObject
    {
        Q_OBJECT
    public:
        explicit Core_Device(QObject *parent = nullptr);
        void moveToThread(QThread*);

    signals:
        void currentPerson(Core::Enums::FirstRes, Core::Person);

    public slots:
        void start();
        void stop();
        void setPersonList(QSet<Core::Person>);
        void receiveCard(IronLogic::Card);

    private:
        IronLogic::Z2USB m_reader;

        Core::Person m_last_person;
        IronLogic::Card m_raw_person;
        QSet<Core::Person> m_allowed_list;

        Q_DISABLE_COPY(Core_Device)
    };
}
#endif // CORE_DEVICE_H
