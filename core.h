#ifndef CORE_H
#define CORE_H

#include "filedownloader.h"
#include "midnighttimer.h"
#include "xmlvalidator.h"
#include "filemanager.h"
#include "simplequery.h"
#include "statistic.h"
#include "imageitem.h"
#include "textfield.h"
#include "xmlreader.h"
#include "person.h"
#include "z2usb.h"
#include "ftp.h"

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
        TextField* getTextProvider();

    signals:

    public slots:
        void runCore();
        void receiveCard(IronLogic::Card); // From serialport
        void receivePersonData(QString); // From network
        void receiveImageData(QString); // From network
        void timeClean(); // From timer to 4h.0.0
        void stop();


    private:
        QString current_date = QDate::currentDate().toString("ddMMyyyy");

        QThread m_serial_port;
        QList<Person> m_person_list;
        //QList<Person> m_person_eat;
        Statistic::Statistic statistic;

        Person last_person;
        IronLogic::Card raw_person;

        IronLogic::Z2USB reader;
        Images::Provider m_image_updater;
        FileManagment::FileManager m_file_manager;
        //Net::FileDownloader m_xmlfile_downloader{"https://lgprk.ru/upload/ftp/utkonos/"};
        //Net::FileDownloader m_imagefile_downloader{"https://cdn.lgprk.ru/users/card/"};
        //Net::FileDownloader m_raw_card{"https://lgprk.ru:/api/v1/food/"};
        Ftp::Ftp m_ftp;
        Utils::MidnightTimer m_midnight_timer;

        QImage not_found {QStringLiteral(":/icons/not_found")};
        QImage forbidden {QStringLiteral(":icons/forbidden")};
        QImage allowed {QStringLiteral(":/icons/allowed")};

        TextField m_text_provide;

        bool DownloadPersons(QString);
        void Init_Reader();
        void Init_Persons();

        void check_eat();
        void save_current_list();
    };
}
#endif // CORE_H
