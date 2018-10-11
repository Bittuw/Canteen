#ifndef CORE_UPDATE_H
#define CORE_UPDATE_H

#include <QObject>
#include <QPair>
#include <QSet>
#include <QDir>

#include "midnighttimer.h"
#include "person.h"
#include "ftp.h"

namespace Core {

    class Core_Update : public QObject
    {
        Q_OBJECT
    public:
        explicit Core_Update(QObject *parent = nullptr);
        void moveToThread(QThread*);

    signals:
        void update();
        void newPersonList(QSet<Core::Person>);

    public slots:
        void start();
        void stop();

        void timeOut();
        void statisticsCreated(QString, QString = "");

    private:
        Utils::MidnightTimer m_midnight_timer;

        QString utkonos = QStringLiteral("utkonos/");
        QString utkonos_archive = QStringLiteral("utkonos/archive/");
        QString utkonos_archive_success = QStringLiteral("utkonos/archive/success/");
        QString utkonos_archive_error = QStringLiteral("utkonos/archive/error/");

        QString sever = QStringLiteral("sever/");
        QString sever_archive = QStringLiteral("sever/archive/");
        QString sever_archive_success = QStringLiteral("sever/archive/success/");
        QString sever_archive_error = QStringLiteral("sever/archive/error/");

        QString date_format = QStringLiteral("ddMMyyyy");
        Ftp::Ftp m_ftp;

        QString m_download_ftp = QDir::currentPath() + QStringLiteral("/clients_utkonos/");

        QSet<QPair<QString, QString>> m_not_loaded_to_ftp; // not uploaded path_file + to_ftp_path
        QSet<QPair<QString, QString>> m_not_downloaded_from_ftp; // path + file_name

        Q_DISABLE_COPY(Core_Update)
    };

}
#endif // CORE_UPDATE_H
