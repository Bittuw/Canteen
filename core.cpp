#include "imageitem.h"
#include "core.h"

#include <QQuickStyle>
#include <QPainter>
#include <QThread>
#include <QDebug>
#include <QDate>


Core::Core::Core(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << "prepare core" << this;

//    forbidden = QImage(QStringLiteral(":/icons/forbidden")).scaled(50,50);
//    allowed = QImage(QStringLiteral(":/icons/allowed")).scaled(50,50);

    qRegisterMetaType<IronLogic::Card>("Card");
    QObject::connect(&m_xmlfile_downloader, &Net::FileDownloader::donwloadFinishedReply,this, &Core::receivePersonData);
    QObject::connect(&m_imagefile_downloader, &Net::FileDownloader::donwloadFinishedReply,this, &Core::receiveImageData);
    m_raw_card.unconnect_for_raw();
}

void Core::Core::moveToThread(QThread *thread) {
    qDebug() << Q_FUNC_INFO << "starting core" << this;

    QObject::connect(thread, &QThread::started, this, &Core::runCore);
    QObject::connect(thread, &QThread::finished, this, &Core::stop);
    QObject::connect(&m_midnight_timer, &Utils::MidnightTimer::TimeOut, this, &Core::Core::timeClean);
    QObject::connect(&reader, &IronLogic::Z2USB::readNewKey, this, &Core::Core::receiveCard);

    QObject::moveToThread(thread);
    m_midnight_timer.moveToThread(thread);
    reader.moveToThread(&m_serial_port);
}

Images::Provider* Core::Core::getProvider() {
    return &m_image_updater;
}

void Core::Core::runCore() {
    qDebug() << Q_FUNC_INFO << "core started." << this;

    Init_Persons();
    Init_Reader();

    m_midnight_timer.start();
}

void Core::Core::receiveCard(IronLogic::Card card) {
    raw_person = card;
    auto i = card.serial.toInt();
    QString string_presenter = QString::number(i, 10) + card.number;
    auto person = std::find_if(m_person_list.cbegin(), m_person_list.cend(),
                 [&string_presenter](const Person& person) -> bool {
                     return person.pass_number == string_presenter;
                 });
    if(person != m_person_list.cend()) {
        last_person = *person;
        check_eat();
        // Есть
    } else {
        m_image_updater.setImage(not_found.scaled(650, 300));
        m_raw_card.raw_card(raw_person);
        // Нет
    }
}

void Core::Core::receivePersonData(QString abs_file_path) {
    qDebug() << Q_FUNC_INFO << "Receive new persons list: " << QFileInfo(abs_file_path).fileName() << this;

    QString file_path = m_file_manager.save_file(abs_file_path);
    file_path = m_file_manager.get_file_info(file_path).absoluteFilePath();

    Xml::XmlValidator validator;
    if(validator.loadSchema("clients_pattern") && validator.validate(file_path)) {

        Xml::XmlReader xml_reader;
        Xml::MessageHandle message_handle;

        xml_reader.setHandle(&message_handle);
        xml_reader.setXmlFile(file_path);
        xml_reader.parse_clients();

        if(message_handle.has_error()) {
            qCritical() << Q_FUNC_INFO << "can not create persons list" << this;
        } else {
            qDebug() << Q_FUNC_INFO << "Loading success xml: " << QFileInfo(file_path).fileName() << this;
            auto info = xml_reader.getInfo();
            m_person_list = info.parce_data.toList();
        }
    } else {
        qCritical() << Q_FUNC_INFO << "can not create persons list" << this;
    }
}

void Core::Core::receiveImageData(QString abs_file_path) {
    auto file_path = m_file_manager.save_file(abs_file_path);
    check_eat();
}

void Core::Core::timeClean() {
    qDebug() << Q_FUNC_INFO << "clean timeout" << this;

    m_midnight_timer.stop();
    //m_person_eat.clear();
    auto new_date = QDate::currentDate().toString("yyyyMMdd");
    auto person_file = new_date + ".xml";
    DownloadPersons(person_file);
    statistic.flush(current_date + ".xml");

    m_midnight_timer.start();
    current_date = new_date;
}

void Core::Core::stop() {
    qDebug() << Q_FUNC_INFO << "core stoped" << this;
    statistic.flush(current_date + "_stat.xml");
    m_serial_port.quit();
    m_serial_port.wait();
    m_midnight_timer.stop();
}

void Core::Core::DownloadPersons(QString xml_file) {
    m_xmlfile_downloader.download(xml_file);
}

void Core::Core::Init_Reader() {
    qDebug() << Q_FUNC_INFO << "inititalize serial port" << this;

    m_serial_port.start();
}

void Core::Core::Init_Persons() {
    qDebug() << Q_FUNC_INFO << "inititalize persons data" << this;
    auto person_file = current_date + ".xml";
    statistic.reestablish(current_date + "_stat.xml");
    if(!m_file_manager.exist_file(person_file)) {
        DownloadPersons(person_file);
    } else {
        receivePersonData(m_file_manager.get_file_info(person_file).absoluteFilePath());
    }
}

void Core::Core::check_eat() {
    if(statistic.contains(last_person)) {

        qDebug() << Q_FUNC_INFO << "person ate:" << last_person.full_name << this;

        if( m_file_manager.exist_file(QUrl(last_person.image_url).fileName())) {

            auto abs_file_path = m_file_manager.get_file_info(QUrl(last_person.image_url).fileName());

            QImage image{abs_file_path.absoluteFilePath()};

            QPainter painter(&image);

            painter.drawImage(
                        QPointF(image.size().width() - forbidden.size().width(), 0),
                        forbidden
                        );
            painter.end();

            qDebug() << Q_FUNC_INFO << QObject::tr("Set negative image: %1").arg(last_person.full_name) << this;

            m_image_updater.setImage(image);
            m_raw_card.raw_card(raw_person);
        } else {
            m_imagefile_downloader.download(QUrl(last_person.image_url).fileName());
        }
    } else {
        if( m_file_manager.exist_file(QUrl(last_person.image_url).fileName())) {
            auto abs_file_path = m_file_manager.get_file_info(QUrl(last_person.image_url).fileName());

            QImage image{abs_file_path.absoluteFilePath()};

            QPainter painter(&image);
            painter.drawImage(
                        QPointF(image.size().width() - allowed.size().width(), 0),
                        allowed
                        );
            painter.end();
            /// не ел
            ///

            qDebug() << Q_FUNC_INFO << QObject::tr("Set positive image: %1").arg(last_person.full_name) << this;

            m_image_updater.setImage(image);
            statistic.add(last_person);
            m_raw_card.raw_card(raw_person);
        } else {
            m_imagefile_downloader.download(QUrl(last_person.image_url).fileName());
        }
    }
}
