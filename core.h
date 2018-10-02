#ifndef CORE_H
#define CORE_H

#include <xmlvalidator.h>
#include <imageitem.h>
#include <z2usb.h>
#include <QObject>
#include <QHash>

namespace Core {

    struct Person {
        int tab_number;
        QString full_name;
        QString serial_number;
        QUrl image_url;
        int discount = 0;

       inline bool operator==(const Person& obj) const {
           return this->tab_number==obj.tab_number;
       }
    };

    inline uint qHash(const Person& person) {
        return ::qHash(person.tab_number);
    }

    class Core : public QObject
    {
        Q_OBJECT
    public:
        explicit Core(QObject *parent = nullptr);
        void moveToThread(QThread*);

    signals:

    public slots:
        void runCore();
        void stop();

    private:
        QThread m_serial_port;
        QList<Person> m_preson_list;
        Xml::XmlValidator m_validator;
        Images::Provider m_image_updater;
    };
}
#endif // CORE_H
