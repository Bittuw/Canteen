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

    m_forbidden_img = m_forbidden_img.scaled(50,50);
    m_allowed_img = m_allowed_img.scaled(50,50);
    m_not_found_img = m_not_found_img.scaled(50,50);

    qRegisterMetaType<IronLogic::Card>("Card");\

    QObject::connect(&m_device, &Core_Device::currentPerson, &m_statistic, &Core_Statistic::receiveCurrentPerson); // Передача полученного пользователя
    QObject::connect(&m_update, &Core_Update::newPersonList, &m_device, &Core_Device::setPersonList); // Установка нового списка пользователей
    QObject::connect(&m_statistic, &Core_Statistic::showPersonInfo, this, &Core::showPersonInfo); // Показать информацию о пользователе (если есть)
    QObject::connect(&m_update, &Core_Update::update, &m_statistic, &Core_Statistic::flush_report); // Сбросить статистику
    QObject::connect(&m_statistic, &Core_Statistic::statisticsCreated, &m_update, &Core_Update::statisticsCreated); // Статистика создана
}

void Core::Core::moveToThread(QThread *thread) {
    qDebug() << Q_FUNC_INFO << "starting core" << this;

    QObject::connect(thread, &QThread::started, this, &Core::start);
    QObject::connect(thread, &QThread::finished, this, &Core::stop);

    m_device.moveToThread(&m_device_statistic_thread);
    m_statistic.moveToThread(&m_device_statistic_thread);
    m_update.moveToThread(&m_update_thread);

    QObject::moveToThread(thread);
}

Images::Provider* Core::Core::getProvider() {
    return &m_image_updater;
}

TextField* Core::Core::getTextProvider() {
    return &m_text_provider;
}

void Core::Core::start() {
    qDebug() << Q_FUNC_INFO << "core started" << this;

    m_device_statistic_thread.start();
    m_update_thread.start();

//    Init_Persons();
//    Init_Reader();
}

void Core::Core::stop() {
    qDebug() << Q_FUNC_INFO << "core stoped" << this;

    m_device_statistic_thread.quit();
    m_update_thread.quit();

    m_device_statistic_thread.wait();
    m_update_thread.wait();
}

void Core::Core::showPersonInfo(Enums::SecondRes status, Person person) {
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::SecondRes>();
    qDebug() << Q_FUNC_INFO <<
                QObject::tr("receive info. Status: '%1', Full name: '%2'")
                .arg(metaEnum.valueToKeys(static_cast<int>(status)).data())
                .arg(person.full_name) << this;

    switch(status) {
    case Enums::SecondRes::ALLOWED:
    {
        m_image_updater.setImage(m_allowed_img);
        m_text_provider.setText(person.full_name);
    }
    case Enums::SecondRes::FORBIDDEN: {
        m_image_updater.setImage(m_forbidden_img);
        m_text_provider.setText(person.full_name);
    }
    case Enums::SecondRes::NOT_FOUND: {
        m_image_updater.setImage(m_not_found_img);
        m_text_provider.setText(QStringLiteral("Not found!"));
    }
    default: {}
    }
}


//void Core::Core::receiveCard(IronLogic::Card card) {
//    //raw_person = card;
//    auto i = card.serial.toInt();
//    QString string_presenter = QString::number(i, 10) + card.number;
//    auto person = std::find_if(m_person_list.cbegin(), m_person_list.cend(),
//                 [&string_presenter](const Person& person) -> bool {
//                     return person.pass_number == string_presenter;
//                 });
//    if(person != m_person_list.cend()) {
//        //last_person = *person;
//        check_eat();
//        // Есть
//    } else {
//        m_image_updater.setImage(not_found);

//        // Нет
//    }
//}

//bool Core::Core::receivePersonData(QString abs_file_path) {
//    qDebug() << Q_FUNC_INFO << "Receive new persons list: " << QFileInfo(abs_file_path).fileName() << this;

//    QString file_path = m_file_manager.save_file(abs_file_path);
//    file_path = m_file_manager.get_file_info(file_path).absoluteFilePath();

//    Xml::XmlValidator validator;
//    if(validator.loadSchema("clients_pattern") && validator.validate(file_path)) {

//        Xml::XmlReader xml_reader;
//        Xml::MessageHandle message_handle;

//        xml_reader.setHandle(&message_handle);
//        xml_reader.setXmlFile(file_path);
//        xml_reader.parse_clients();

//        if(message_handle.has_error()) {
//            qCritical() << Q_FUNC_INFO << "can not create persons list" << this;
//        } else {
//            qDebug() << Q_FUNC_INFO << "Loading success xml: " << QFileInfo(file_path).fileName() << this;
//            auto info = xml_reader.getInfo();
//            m_person_list = info.parce_data.toList();
//            m_ftp.upload_file(file_path, "sever/archive/success/"); // После успешной обработки
//        }
//    } else {
//        m_ftp.upload_file(file_path, "sever/archive/error/"); // После неуспешной обработки
//        qCritical() << Q_FUNC_INFO << "can not create persons list" << this;
//    }
//}

//void Core::Core::receiveImageData(QString abs_file_path) {
//    auto file_path = m_file_manager.save_file(abs_file_path);
//    check_eat();
//}

//void Core::Core::timeClean() {
//    qDebug() << Q_FUNC_INFO << "clean timeout" << this;

//    m_midnight_timer.stop();
//    //m_person_eat.clear();
//    auto new_date = QDate::currentDate().toString("yyyyMMdd");
//    auto person_file = new_date + ".xml";
//    DownloadPersons(person_file);
//    statistic.flush(current_date + ".xml");

//    m_midnight_timer.start();
//    current_date = new_date;
//}



//bool Core::Core::DownloadPersons(QString xml_file) {
//    auto new_file = m_file_manager.getDir() + xml_file;
//    return m_ftp.download_fille(xml_file, new_file);
//}

//void Core::Core::Init_Reader() {
//    qDebug() << Q_FUNC_INFO << "inititalize serial port" << this;
//    m_serial_port.start();
//}

//void Core::Core::Init_Persons() {
//    qDebug() << Q_FUNC_INFO << "inititalize persons data" << this;

//    auto person_file = "client_" + current_date + ".xml";
//    statistic.reestablish("client_" + current_date + "_stat.xml");

//    if(!m_file_manager.exist_file(person_file)) {
//        if(DownloadPersons(person_file) && m_file_manager.exist_file(person_file)) {
//            receivePersonData(m_file_manager.get_file_info(person_file).absoluteFilePath());
//        } else {
//            QString error("ERROR: can not load xml!");
//            qCritical() << Q_FUNC_INFO << error << this;
//            m_text_provider.setText(error);
//        }
//    } else {
//        receivePersonData(m_file_manager.get_file_info(person_file).absoluteFilePath());
//    }
//}

//void Core::Core::check_eat() {
//    if(statistic.contains(last_person)) {

//        qDebug() << Q_FUNC_INFO << "person ate:" << last_person.full_name << this;

//        if( m_file_manager.exist_file(QUrl(last_person.image_url).fileName())) {

//            auto abs_file_path = m_file_manager.get_file_info(QUrl(last_person.image_url).fileName());

//            QImage image{abs_file_path.absoluteFilePath()};

//            QPainter painter(&image);

//            painter.drawImage(
//                        QPointF(image.size().width() - forbidden.size().width(), 0),
//                        forbidden
//                        );
//            painter.end();

//            qDebug() << Q_FUNC_INFO << QObject::tr("Set negative image: %1").arg(last_person.full_name) << this;

//            m_image_updater.setImage(image);
//            //m_raw_card.raw_card(raw_person);
//        } else {
//            //m_imagefile_downloader.download(QUrl(last_person.image_url).fileName());
//        }
//    } else {
//        if( m_file_manager.exist_file(QUrl(last_person.image_url).fileName())) {
//            auto abs_file_path = m_file_manager.get_file_info(QUrl(last_person.image_url).fileName());

//            QImage image{abs_file_path.absoluteFilePath()};

//            QPainter painter(&image);
//            painter.drawImage(
//                        QPointF(image.size().width() - allowed.size().width(), 0),
//                        allowed
//                        );
//            painter.end();
//            /// не ел
//            ///

//            qDebug() << Q_FUNC_INFO << QObject::tr("Set positive image: %1").arg(last_person.full_name) << this;

//            m_image_updater.setImage(image);
//            statistic.add(last_person);
//            //m_raw_card.raw_card(raw_person);
//        } else {
//            //m_imagefile_downloader.download(QUrl(last_person.image_url).fileName());
//        }
//    }
//}


//void Core::Core::update() {
//    auto person_file = "client_" + current_date + ".xml";
//    if(DownloadPersons(person_file) && m_file_manager.exist_file(person_file)) {
//        receivePersonData(m_file_manager.get_file_info(person_file).absoluteFilePath());
//    } else {
//        QString error("ERROR: can not load xml!");
//        qCritical() << Q_FUNC_INFO << error << this;
//        m_text_provider.setText(error);
//    }
//}
