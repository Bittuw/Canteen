#include "filedownloader.h"

#include <functional>

#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QFile>
#include <QDir>

Net::FileDownloader::FileDownloader(QString base_url, QObject *parent) : QObject(parent),
    default_path(QDir::currentPath() + "/temp/"), m_base_url(base_url)
{
    if(!QDir(QDir::currentPath()).exists("temp"))
        QDir().mkdir("temp");

    qDebug() << Q_FUNC_INFO << "init FileDonwloader" << this;

    connect(&manager, &QNetworkAccessManager::finished, this, &FileDownloader::downloadFinished);
    connect(&manager, &QNetworkAccessManager::sslErrors, this, &FileDownloader::sslError);
    connect(&manager, &QNetworkAccessManager::encrypted, this, &FileDownloader::encrypted);
}

void Net::FileDownloader::download(QString file) {
    qDebug() << Q_FUNC_INFO << "start loading file: " + file << this;
    QUrl url;
    url.setUrl(m_base_url.toString() + file);
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    QNetworkRequest request;
    request.setSslConfiguration(config);
    request.setUrl(url);
    auto reply = manager.get(request);
    Q_UNUSED(reply)
}

void Net::FileDownloader::raw_card(IronLogic::Card card) {
    qDebug() << Q_FUNC_INFO << "Raw query: " + card.number << card.serial << this;
    QUrl url;
    url.setUrl(m_base_url.toString() + QObject::tr("?key=d412423r4sdrfsdfdsfsf&serial=%1&number=%2&place=1")
               .arg(card.serial)
               .arg(card.number)
               );
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    QNetworkRequest request;
    request.setSslConfiguration(config);
    request.setUrl(url);
    manager.get(request);
}

void Net::FileDownloader::unconnect_for_raw() {
   QObject::disconnect(&manager, &QNetworkAccessManager::finished, this, &FileDownloader::downloadFinished);
}

Net::FileDownloader::~FileDownloader() {
    QDir(default_path).removeRecursively();
}

void Net::FileDownloader::downloadFinished(QNetworkReply *reply) {
    qDebug() << Q_FUNC_INFO << QObject::tr("saving file %1 in directory: %2")
                .arg(QFileInfo(reply->url().path()).fileName())
                .arg(default_path) << this;
    QFile new_file;
    new_file.setFileName(default_path + QFileInfo(reply->url().path()).fileName());
    new_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    new_file.write(reply->readAll());
    new_file.close();
    emit donwloadFinishedReply(QFileInfo(new_file).absoluteFilePath());
}

void Net::FileDownloader::sslError(QNetworkReply* reply, const QList<QSslError> &errors) {
    reply->ignoreSslErrors();
    for(auto& error : errors)
        qDebug() << Q_FUNC_INFO << error.errorString() << this;
}

void Net::FileDownloader::encrypted(QNetworkReply* reply) {
    qDebug() << Q_FUNC_INFO << "Succsessful connected to host: " << reply->url().host() << this;
}
