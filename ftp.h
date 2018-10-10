#ifndef FTP_H
#define FTP_H

#include <QObject>

#include "Poco/Net/FTPClientSession.h"

namespace Ftp {

    class Ftp : public QObject
    {
    Q_OBJECT
    public:
        Ftp(QObject *parent = nullptr);
        bool download_fille(QString&, QString&);
        bool upload_file(QString&, QString&);
        ~Ftp();
    private:
        Poco::Net::FTPClientSession output;
        Poco::Net::FTPClientSession input;
    };

}
#endif // FTP_H
