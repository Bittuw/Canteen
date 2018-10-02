#include "filedownloader.h"

#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QFile>
#include <QDir>

FileDownloader::FileDownloader(QString url, QObject *parent) : QObject(parent),
    default_path(QDir::currentPath() + "/temp/"), url(url)
{
    if(QDir(QDir::currentPath()).exists())
        QDir().mkdir("temp");
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    QNetworkRequest request;
    request.setSslConfiguration(config);
    request.setUrl(url);
    qDebug() << Q_FUNC_INFO << "start loading file:" << QFile(this->url.path()).fileName() << this;
    manager.get(request);
    connect(&manager, &QNetworkAccessManager::finished, this, &FileDownloader::downloadFinished);
    connect(&manager, &QNetworkAccessManager::sslErrors, this, &FileDownloader::sslError);
    connect(&manager, &QNetworkAccessManager::encrypted, this, &FileDownloader::encrypted);
}

void FileDownloader::downloadFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << "saving file in directory:" << default_path << this;
    QFile new_file;
    new_file.setFileName(default_path + QFileInfo(url.path()).fileName());
    new_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    new_file.write(reply->readAll());
    new_file.close();
    emit donwloadFinishedReply(QFileInfo(new_file).absoluteFilePath());
}

void FileDownloader::sslError(QNetworkReply* reply, const QList<QSslError> &errors) {
    reply->ignoreSslErrors();
    for(auto& error : errors)
        qDebug() << Q_FUNC_INFO << error.errorString() << this;
}

void FileDownloader::encrypted(QNetworkReply* reply) {
    Q_UNUSED(reply)
    qDebug() << Q_FUNC_INFO << "connected to host:" << url.host() << this;
}
