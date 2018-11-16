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
    //QObject::connect(&m_midnight_timer, &Utils::MidnightTimer::TimeOut, this, &Core_Update::timeOut);
    QObject::connect(&m_hour_timer, &Utils::MidnightTimer::TimeOut, this, &Core_Update::hourTimeOut);

    //m_midnight_timer.moveToThread(thread);
    m_hour_timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Core::Core_Update::start() {
    qDebug() << Q_FUNC_INFO << QObject::tr("start %1").arg(this->metaObject()->className()) << this;
    auto time = QTime::currentTime();
    //m_midnight_timer.start();
    m_hour_timer.start(TO_NEXT_HOUR(time.second(), time.minute()) );
    //current_date = QDateTime::currentDateTime();
}

void Core::Core_Update::stop() {
    qDebug() << Q_FUNC_INFO << QObject::tr("stop %1").arg(this->metaObject()->className()) << this;
    m_hour_timer.stop();
    //m_midnight_timer.stop();
}


// обновление каждый день
//void Core::Core_Update::timeOut(QDateTime time_point) {
//    m_midnight_timer.stop();

//    if(transitions_report_hour != 23) {
//        hourTimeOut(time_point); // force transition
//    }

//    auto time = time_point;

//    if(current_date < time_point) {
//        emit makeEndDayStatistics(current_date.toString("yyyy-MM-dd 23:59:59")); // force reports
//        emit ClearDay(); // clear day stats
//    } else {
//        emit makeEndDayStatistics(time.toString("yyyy-MM-dd 23:59:59")); // force reports
//        emit ClearDay(); // clear day stats
//    }

//    qInfo() << Q_FUNC_INFO << "Day end at" << time.toString("dd-MM-yyyy 23:59:59") << this;

//    current_date = QDateTime::currentDateTime();

//    if(current_date.date() < time_point.date())
//            while(true) {
//                if(time.time().minute() > 0) {
//                    QThread::sleep(static_cast<unsigned long>(60 - time.time().second()));
//                    current_date = QDateTime::currentDateTime();
//                } else break;
//            }

//    if(check_ethernet()) updating();
//    m_midnight_timer.start();
//}

// статистика каждый час
void Core::Core_Update::hourTimeOut(QDateTime time_point) {
    m_hour_timer.stop();

    emit makeTransitionStatistics(time_point.toString("yyyy-MM-dd hh:mm:ss")); // force reports

    if(time_point.time().hour() == 23) {
        emit makeEndDayStatistics(time_point.toString("yyyy-MM-dd hh:mm:ss"));
        emit ClearDay();
        if(check_ethernet()) updating(time_point.addDays(1));
    }

    auto time = QDateTime::currentDateTime();

    if(time.time().hour() != time_point.time().hour()) {
                m_hour_timer.start(
                            TO_NEXT_HOUR(
                                time.time().second(),
                                time.time().minute())
                            );
    }
    else {
                m_hour_timer.start(
                        TO_NEXT_HOUR(
                            (time.time().second() - 59),
                            (time.time().minute() - 59)
                        )
                    );
    }

    qInfo() << Q_FUNC_INFO << "To next hour is:" << TO_NEXT_HOUR(time.time().second(), time.time().minute()) << this;
}

// первичная загрузка
void Core::Core_Update::forceUpdate() {
    if(check_ethernet()) updating();
}

void Core::Core_Update::statisticsCreated(QString abs_sales_report_file, QString abs_menu_file) {

    qDebug() << Q_FUNC_INFO << QObject::tr("upload file: '%1' AND '%2'").arg(abs_sales_report_file).arg(abs_menu_file) << this;

    /// Залить репорт
    if(/*!QFile(abs_sales_report_file).exists() && */!m_ftp.upload_file(abs_sales_report_file, sever))
        qWarning() << Q_FUNC_INFO << QObject::tr("Can not upload report file '%1'").arg(QFileInfo(abs_sales_report_file).fileName()) << this;
//    else
//        QFile(abs_sales_report_file).remove();

    if(/*!QFile(abs_menu_file).exists() && */!m_ftp.upload_file(abs_menu_file, sever))
        qWarning() << Q_FUNC_INFO << QObject::tr("can not upload menu file '%1'").arg(QFileInfo(abs_menu_file).fileName()) << this;
//    else
//        QFile(abs_menu_file).remove();

    emit UploadDateTime(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
}

void Core::Core_Update::updating(QDateTime time_point) {

    auto current_date =  time_point.toString(date_format);
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

