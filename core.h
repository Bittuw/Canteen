#ifndef CORE_H
#define CORE_H

#include "imageitem.h"
#include "textfield.h"
#include "person.h"

#include "core_statistic.h"
#include "core_update.h"
#include "core_device.h"

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
        void start();
        void stop();
        void showPersonInfo(Enums::SecondRes,Person);

    private:
        Images::Provider m_image_updater;
        TextField m_text_provider;

        QThread m_device_statistic_thread;
        QThread m_update_thread;

        Core_Statistic m_statistic;
        Core_Update m_update;
        Core_Device m_device;


        QImage m_not_found_img {QStringLiteral(":/icons/not_found")};
        QImage m_forbidden_img {QStringLiteral(":icons/forbidden")};
        QImage m_allowed_img {QStringLiteral(":/icons/allowed")};


//        bool DownloadPersons(QString);
//        void Init_Reader();
//        void Init_Persons();

//        void check_eat();
//        void save_current_list();
//        void update();
    };
}
#endif // CORE_H
