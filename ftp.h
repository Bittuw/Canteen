#ifndef FTP_H
#define FTP_H

#include <QObject>

#include "Poco/Net/FTPClientSession.h"

class QFile;

namespace Ftp {

    class Ftp : public QObject
    {
    Q_OBJECT
    public:
        Ftp(QObject *parent = nullptr);
        bool download_file(QString, QString);
        bool upload_file(QString, QString, QString = "");
        void move_file(QString, QString, QString);
        void delete_file(QString, QString);
    private:
        QString host;
        QString user;
        QString passwd;
        Poco::Int16 port;

        void blocking_download_file(Poco::Net::FTPClientSession&, QFile&, QString&);

    };

}
#endif // FTP_H
