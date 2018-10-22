#include "core_update.h"

#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDir>

#include "Poco/Net/DNS.h"
#include "xmlvalidator.h"
#include "xmlreader.h"

Core::Core_Update::Core_Update(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << QObject::tr("create %1 at").arg(this->metaObject()->className()) << this;
    QDir::current().mkdir(QStringLiteral("clients_utkonos"));
}

void Core::Core_Update::moveToThread(QThread* thread) {
    qDebug() << Q_FUNC_INFO << QObject::tr("moving %1 to thread").arg(this->metaObject()->className()) << thread << this;

    QObject::connect(thread, &QThread::started, this, &Core_Update::start);
    QObject::connect(thread, &QThread::finished, this, &Core_Update::stop);
    QObject::connect(&m_midnight_timer, &Utils::MidnightTimer::TimeOut, this, &Core_Update::timeOut);
    QObject::connect(&m_hour_timer, &Utils::MidnightTimer::TimeOut, this, &Core_Update::hourTimeOut);

    m_midnight_timer.moveToThread(thread);
    m_hour_timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Core::Core_Update::start() {
    qDebug() << Q_FUNC_INFO << QObject::tr("start %1").arg(this->metaObject()->className()) << this;
    auto time = QTime::currentTime();
    m_hour_timer.start(TO_NEXT_HOUR(time.second(), time.minute()));
    m_midnight_timer.start();
}

void Core::Core_Update::stop() {
    qDebug() << Q_FUNC_INFO << QObject::tr("stop %1").arg(this->metaObject()->className()) << this;
    m_midnight_timer.stop();
    m_hour_timer.stop();
}


// обновление каждый день
void Core::Core_Update::timeOut() {
    qInfo() << Q_FUNC_INFO << "Day end at" << QDateTime::currentDateTime().toString("dd-MM-yyyy") << this;
    emit makeEndDayStatistics(); // force reports
    emit ClearDay(); // clear day stats
    if(check_ethernet()) updating();
}


// статистика каждый час
void Core::Core_Update::hourTimeOut() {
    emit makeTransitionStatistics(); // force reports
}

// первичная
void Core::Core_Update::forceUpdate() {
    if(check_ethernet()) updating();
}

void Core::Core_Update::statisticsCreated(QString abs_sales_report_file, QString abs_menu_file) {

    qDebug() << Q_FUNC_INFO << QObject::tr("upload file: '%1' AND '%2'").arg(abs_sales_report_file).arg(abs_menu_file) << this;

    /// Залить репор
    if(!m_ftp.upload_file(abs_sales_report_file, sever))
        qWarning() << Q_FUNC_INFO << QObject::tr("can not upload file '%1'").arg(QFileInfo(abs_sales_report_file).fileName()) << this;
//    else
//        QFile(abs_sales_report_file).remove();

    if(m_ftp.upload_file(abs_menu_file, sever))
        qWarning() << Q_FUNC_INFO << QObject::tr("can not upload file '%1'").arg(QFileInfo(abs_menu_file).fileName()) << this;
//    else
//        QFile(abs_menu_file).remove();

    emit UploadDateTime(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm::ss"));
}

void Core::Core_Update::updating() {

    auto current_date =  QDate::currentDate().toString(date_format);
    auto current_xml_file = "client_" + current_date + ".xml";
    auto abs_current_xml_file = m_download_ftp + current_xml_file;

    if(m_ftp.download_file(utkonos, m_download_ftp + current_xml_file)) {

        Xml::MessageHandle message_handle;
        Xml::XmlValidator validator;
        Xml::XmlReader xml_reader;

        xml_reader.setHandle(&message_handle);
        xml_reader.setXmlFile(abs_current_xml_file);

        if(validator.loadSchema("clients_pattern") && validator.validate(abs_current_xml_file)) {
            xml_reader.parse_clients();
            if(!message_handle.has_error()) {
                qDebug() << Q_FUNC_INFO << "Loading success xml: " << current_xml_file << this;
                emit newPersonList(xml_reader.getInfo().parce_data);
                emit DownloadDate(QDate::currentDate().toString("dd-MM-yyyy"));
                //m_ftp.move_file(utkonos, current_xml_file, utkonos_archive_success + current_xml_file); //Или следующее условие
                //QFile(abs_current_xml_file).remove();
            } else {
                //m_ftp.move_file(utkonos, current_xml_file, utkonos_archive_error + current_xml_file);
                qCritical() << Q_FUNC_INFO << QObject::tr("can not create persons list from '%1'").arg(current_xml_file) << this; // Неупешный парсинг
            }

        } else {
            //m_ftp.move_file(utkonos, current_xml_file, utkonos_archive_error + current_xml_file);
            qWarning() << Q_FUNC_INFO << QObject::tr("can not validate file '%1'").arg(current_xml_file) << this; // После неуспешной обработки
        }
    } else {

        qCritical() << Q_FUNC_INFO << QObject::tr("can not load file '%1'").arg(current_xml_file) << this;
    }
}

bool Core::Core_Update::check_ethernet() {
    try {
        QSettings ftp_settings(QDir::currentPath() + "/ftp.ini", QSettings::IniFormat);
        auto host = ftp_settings.value("FTP/host").toString();
        auto dns_resolving = Poco::Net::DNS::hostByName(host.toStdString());
    } catch(std::exception& error) {
        qWarning() << Q_FUNC_INFO << error.what() << this;
        return false;
    }
    return true;
}

