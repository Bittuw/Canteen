#include "imageitem.h"
#include "core.h"

#include <QQuickStyle>
#include <QPainter>
#include <QThread>
#include <QVector>
#include <QDebug>
#include <QDate>


Core::Core::Core(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << "Prepare core" << this;

    qRegisterMetaType<IronLogic::Card>();
    qRegisterMetaType<QSet<Person>>();
    qRegisterMetaType<QVector<Person>>();
    qRegisterMetaType<Enums::FirstRes>();
    qRegisterMetaType<Enums::SecondRes>();

    QObject::connect(&m_device, &Core_Device::currentPerson, &m_statistic, &Core_Statistic::receiveCurrentPerson); // Передача полученного пользователя
    QObject::connect(&m_update, &Core_Update::newPersonList, &m_device, &Core_Device::setPersonList); // Установка нового списка пользователей
    QObject::connect(&m_statistic, &Core_Statistic::showPersonInfo, this, &Core::showPersonInfo); // Показать информацию о пользователе (если есть)
    QObject::connect(&m_update, &Core_Update::makeEndDayStatistics, &m_statistic, &Core_Statistic::flush_report); // Сбросить статистику
    QObject::connect(&m_update, &Core_Update::makeTransitionStatistics, &m_statistic, &Core_Statistic::flush_transitional_report); // ПРомежуточная статистика
    QObject::connect(&m_statistic, &Core_Statistic::madeEndDayStatistics, &m_update, &Core_Update::statisticsCreated); // Статистика создана
    QObject::connect(&m_statistic, &Core_Statistic::madeTransitionStatistics, &m_update, &Core_Update::statisticsCreated); // Статистика создана
    QObject::connect(&m_update, &Core_Update::DownloadDate, [this](QString msg){m_text_provider_status.setText(msg); });
    QObject::connect(&m_update, &Core_Update::UploadDateTime, [this](QString msg){m_text_provider_datetime.setText(msg);});
    QObject::connect(this, &Core::force_ftp_update, &m_update, &Core_Update::forceUpdate);

    /*QObject::connect(this,&Core::test_receiveCard, &m_device, &Core_Device::receiveCard); // test
    QObject::connect(&m_update, &Core_Update::newPersonList, this, &Core::setNewList);*/ // Установка нового списка пользователей
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

TextField* Core::Core::getTextProviderStatus() {
    return &m_text_provider_status;
}

TextField* Core::Core::getTextProviderDateTime() {
    return &m_text_provider_datetime;
}

QObject* Core::Core::getComplexProvider() {
    return &m_statistic;
}

void Core::Core::start() {
    qDebug() << Q_FUNC_INFO << "core started" << this;

    m_device_statistic_thread.start();
    m_update_thread.start();
    emit force_ftp_update();

}

void Core::Core::stop() {
    qDebug() << Q_FUNC_INFO << "core stoped" << this;

    m_device_statistic_thread.quit();
    m_update_thread.quit();

    m_device_statistic_thread.wait();
    m_update_thread.wait();
}

//void Core::Core::setNewList(QSet<Person> pe) {
//    Q_UNUSED(pe);
//    emit test_receiveCard({"83", "1", ""});
//}

void Core::Core::showPersonInfo(Enums::SecondRes status, Person person) {
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::SecondRes>();
    qInfo() << Q_FUNC_INFO <<
                QObject::tr("Receive info. Status: '%1', Full name: '%2'")
                .arg(metaEnum.valueToKeys(static_cast<int>(status)).data())
                .arg(person.full_name) << this;

    switch(status) {
    case Enums::SecondRes::ALLOWED:
    {
        m_image_updater.setImage(m_allowed_img);
        m_text_provider.setText(person.full_name);
        break;
    }
    case Enums::SecondRes::FORBIDDEN: {
        m_image_updater.setImage(m_forbidden_img);
        m_text_provider.setText(person.full_name);
        break;
    }
    case Enums::SecondRes::NOT_FOUND: {
        m_image_updater.setImage(m_not_found_img);
        m_text_provider.setText(QStringLiteral("Not found or unrecognized, try again!"));
        break;
    }
    default: {

    }
    }
}
