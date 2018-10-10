#include "ftp.h"

#include <iostream>
#include <sstream>

#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QDir>
//
#include "Poco/Net/FTPClientSession.h"

Ftp::Ftp::Ftp(QObject *parent) : QObject(parent)
{
    QSettings ftp_settings(QDir::currentPath() + "/ftp.ini", QSettings::NativeFormat);

    auto out_host = ftp_settings.value("FTP_OUT/host").toString();
    auto out_user = ftp_settings.value("FTP_OUT/user").toString();
    auto out_passwd = ftp_settings.value("FTP_OUT/passwd").toString();

    auto in_host = ftp_settings.value("FTP_IN/host").toString();
    auto in_user = ftp_settings.value("FTP_IN/user").toString();
    auto in_passwd = ftp_settings.value("FTP_IN/passwd").toString();

    output.open(
                out_host.toStdString(),
                Poco::Net::FTPClientSession::FTP_PORT,
                out_user.toStdString(), out_passwd.toStdString()
    );
    input.open(
                in_host.toStdString(),
                Poco::Net::FTPClientSession::FTP_PORT,
                in_user.toStdString(), in_passwd.toStdString()
    );
}

bool Ftp::Ftp::download_fille(QString& ftp_path, QString& local_path) {
    QFile in_file(local_path);
    if(input.isOpen() && in_file.open(QIODevice::WriteOnly)) {
        try {
            auto& in_stream = input.beginDownload(ftp_path.toStdString());
            std::stringstream in_data;
            while(in_stream) {
                in_data << in_stream.get();
            }
            input.endDownload();
            QByteArray in_qdata = QByteArray::fromStdString(in_data.str());
            in_file.write(in_qdata);
            return true;
        } catch(const std::exception& error) {
            qWarning() << Q_FUNC_INFO << error.what() << this;
            return false;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "cannot download file: " << ftp_path << this;
        return false;
    }
}

bool Ftp::Ftp::upload_file(QString& local_path, QString& ftp_path) {
    QFile out_file(local_path);
    if(output.isOpen() && out_file.open(QIODevice::ReadOnly)) {
        try {
            auto& out_stream = output.beginUpload(ftp_path.toStdString());
            out_stream << out_file.readAll().toStdString();
            output.endUpload();
        }   catch(const std::exception& error) {
            qWarning() << Q_FUNC_INFO << error.what() << this;
            return false;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "cannot upload file: " << local_path << this;
        return false;
    }
}


Ftp::Ftp::~Ftp() {
    output.close();
    input.close();
}
