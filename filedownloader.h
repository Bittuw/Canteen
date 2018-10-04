#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QObject>

#include "z2usb.h"

namespace Net {
    class FileDownloader : public QObject
    {
        Q_OBJECT
    public:
        explicit FileDownloader(QString url, QObject *parent = nullptr);
        void download(QString);
        void raw_card(IronLogic::Card);
        void unconnect_for_raw();
    signals:
        void donwloadFinishedReply(QString);
    public slots:
        void downloadFinished(QNetworkReply*);
        void encrypted(QNetworkReply*);
        void sslError(QNetworkReply*, const QList<QSslError>&);
    private:
        QNetworkAccessManager manager;
        QString default_path;
        QString file_name;
        QUrl m_base_url;
    };
}
#endif // FILEDOWNLOADER_H
