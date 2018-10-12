#include "core_update.h"

#include <QDebug>
#include <QDate>
#include <QDir>

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

    m_midnight_timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Core::Core_Update::start() {
    qDebug() << Q_FUNC_INFO << QObject::tr("start %1").arg(this->metaObject()->className()) << this;
    m_midnight_timer.start();
}

void Core::Core_Update::stop() {
    qDebug() << Q_FUNC_INFO << QObject::tr("stop %1").arg(this->metaObject()->className()) << this;
    m_midnight_timer.stop();
}

void Core::Core_Update::timeOut() {
    emit update(); // force reports
    updating();
}

void Core::Core_Update::forceUpdate() {
    updating();
}

void Core::Core_Update::statisticsCreated(QString abs_sales_report_file, QString abs_menu_file) {
    Q_UNUSED(abs_menu_file) // temp

    if(!m_ftp.upload_file(sever, abs_sales_report_file))
        qWarning() << Q_FUNC_INFO << QObject::tr("can not upload file '%1'").arg(QFileInfo(abs_sales_report_file).fileName()) << this;
    else
        QFile(abs_sales_report_file).remove();
//    if(m_ftp.upload_file(sever, abs_menu_file))
//        qWarning() << Q_FUNC_INFO << QObject::tr("can not upload file '%1'").arg(QFileInfo(abs_menu_file).fileName()) << this;
        //QFile(abs_menu_file).remove();
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
            if(xml_reader.parse_clients(); !message_handle.has_error()) {
                qDebug() << Q_FUNC_INFO << "Loading success xml: " << current_xml_file << this;
                emit newPersonList(xml_reader.getInfo().parce_data);
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
