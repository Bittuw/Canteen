#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QObject>


class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QString url, QObject *parent = nullptr);
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
    QUrl url;
};

#endif // FILEDOWNLOADER_H
