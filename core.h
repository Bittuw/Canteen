#ifndef CORE_H
#define CORE_H

#include "filedownloader.h"
#include "midnighttimer.h"
#include "xmlvalidator.h"
#include "filemanager.h"
#include "simplequery.h"
#include "imageitem.h"
#include "xmlreader.h"
#include "person.h"
#include "z2usb.h"

#include <QObject>
#include <QHash>

namespace Core {

    class Core : public QObject
    {
        Q_OBJECT
    public:
        explicit Core(QObject *parent = nullptr);
        void moveToThread(QThread*);
        Images::Provider* getProvider();

    signals:

    public slots:
        void runCore();
        void receiveCard(IronLogic::Card); // From serialport
        void receivePersonData(QString); // From network
        void receiveImageData(QString); // From network
        void timeClean(); // From timer to 0.0.0
        void stop();

    private:
        QThread m_serial_port;
        QList<Person> m_person_list;
        QList<Person> m_person_eat;

        Person last_person;
        IronLogic::Card raw_person;

        IronLogic::Z2USB reader;
        //Xml::XmlValidator m_validator;
        Images::Provider m_image_updater;
        FileManagment::FileManager m_file_manager;
        Net::FileDownloader m_xmlfile_downloader{"https://lgprk.ru/upload/ftp/utkonos/"};
        Net::FileDownloader m_imagefile_downloader{"https://cdn.lgprk.ru/users/card/"};
        Net::FileDownloader m_raw_card{"https://lgprk.ru:/api/v1/food/"};

        Utils::MidnightTimer m_midnight_timer;
        void DownloadPersons(QString);
        void Init_Reader();
        void Init_Persons();

        void check_eat();
        void save_current_list();
    };
}
#endif // CORE_H
