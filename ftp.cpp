#include "ftp.h"

#include <iostream>
#include <sstream>

#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QUrl>
//
#include "Poco/Net/FTPClientSession.h"
#include "Poco/Net/DNS.h"

Ftp::Ftp::Ftp(QObject *parent) : QObject(parent)
{
    QSettings ftp_settings(QDir::currentPath() + "/ftp.ini", QSettings::IniFormat);
    host = ftp_settings.value("FTP/host").toString();
    port = static_cast<Poco::Int16>(ftp_settings.value("FTP/port").toInt());
    user = ftp_settings.value("FTP/user").toString();
    passwd = ftp_settings.value("FTP/passwd").toString();
}

///
///
/// ftp_path путь до папки, local_path - абс путь до файла
///
///
bool Ftp::Ftp::download_file(QString ftp_path, QString local_path) {
    Poco::Net::FTPClientSession ftp;

    try {
        ftp.open(
                    host.toStdString(),
                    static_cast<Poco::UInt16>(port),
                    user.toStdString(),
                    passwd.toStdString()
        );
    } catch(const std::exception& error) {
        qCritical() << Q_FUNC_INFO << error.what() << this;
        return false;
    }

    QFile in_file(local_path);

    if(ftp.isOpen() && in_file.open(QIODevice::WriteOnly)) {
        try {
            ftp.setWorkingDirectory(ftp_path.toStdString());
            auto& in_stream = ftp.beginDownload(in_file.fileName().toStdString());

            std::stringstream in_data;
            while(in_stream) {
                in_data << in_stream.get();
            }

            ftp.endDownload();
            //ftp.cdup();

            QByteArray in_qdata = QByteArray::fromStdString(in_data.str());
            in_file.write(in_qdata);
            in_file.close();
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

bool Ftp::Ftp::upload_file(QString local_path, QString ftp_path, QString rename_file) {
    Poco::Net::FTPClientSession ftp;

    try {
        ftp.open(
                    host.toStdString(),
                    static_cast<Poco::UInt16>(port),
                    user.toStdString(),
                    passwd.toStdString()
        );
    } catch(const std::exception& error) {
        qCritical() << Q_FUNC_INFO << error.what() << this;
        return false;
    }

    QFile out_file(local_path);

    if(ftp.isOpen() && out_file.open(QIODevice::ReadOnly)) {
        try {

            rename_file.isEmpty() ? rename_file = out_file.fileName() : nullptr;

            ftp.setWorkingDirectory(ftp_path.toStdString());
            auto& out_stream = ftp.beginUpload(rename_file.toStdString());

            out_stream << out_file.readAll().toStdString();

            ftp.endUpload();
            //ftp.cdup();

            out_file.close();
            return true;
        } catch(const std::exception& error) {
            qWarning() << Q_FUNC_INFO << error.what() << this;
            return false;
        }
    } else {
        qWarning() << Q_FUNC_INFO << "cannot upload file: " << local_path << this;
        return false;
    }
}

void Ftp::Ftp::move_file(QString ftp_path, QString ftp_file, QString new_file_path) {
    Poco::Net::FTPClientSession ftp;

    try {
        ftp.open(
                    host.toStdString(),
                    static_cast<Poco::UInt16>(port),
                    user.toStdString(),
                    passwd.toStdString()
        );
    } catch(const std::exception& error) {
        qCritical() << Q_FUNC_INFO << error.what() << this;
    }

    if(ftp.isOpen()) {
        ftp.setWorkingDirectory(ftp_path.toStdString());
        ftp.rename(ftp_file.toStdString(), new_file_path.toStdString());
    }

    ftp.close();
}

void Ftp::Ftp::delete_file(QString ftp_path, QString ftp_file) {
    Poco::Net::FTPClientSession ftp;

    try {
        ftp.open(
                    host.toStdString(),
                    static_cast<Poco::UInt16>(port),
                    user.toStdString(),
                    passwd.toStdString()
        );
    } catch(const std::exception& error) {
        qCritical() << Q_FUNC_INFO << error.what() << this;
    }

    if(ftp.isOpen()) {
        ftp.setWorkingDirectory(ftp_path.toStdString());
        ftp.remove(ftp_file.toStdString());
    }

    ftp.close();
}
