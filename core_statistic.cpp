#include "core_statistic.h"

#include <QDateTime>
#include <QMetaEnum>
#include <QThread>
#include <QDebug>

Core::Core_Statistic::Core_Statistic(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << QObject::tr("create %1 at").arg(this->metaObject()->className()) << this;
}

void Core::Core_Statistic::moveToThread(QThread* thread) {
    qDebug() << Q_FUNC_INFO << QObject::tr("moving %1 to thret:ad").arg(this->metaObject()->className()) << thread << this;

    QObject::connect(thread, &QThread::started, this, &Core_Statistic::start);
    QObject::connect(thread, &QThread::finished, this, &Core_Statistic::stop);

    QObject::moveToThread(thread);
}

void Core::Core_Statistic::start() {
    qDebug() << Q_FUNC_INFO << QObject::tr("start %1").arg(this->metaObject()->className()) << this;
    start_date = QDateTime::currentDateTime().toString(date_format);
    start_time = QDateTime::currentDateTime().toString(datetime_format);
    m_sales_report.reestablish(start_date + QStringLiteral("_abort"));
}

void Core::Core_Statistic::stop() {
    end_time = QDateTime::currentDateTime().toString(datetime_format);
    if(start_date == QDateTime::currentDateTime().toString(date_format))
                m_sales_report.flush(start_date + QStringLiteral("_abort"), start_time, end_time);
    qDebug() << Q_FUNC_INFO << QObject::tr("stop %1").arg(this->metaObject()->className()) << this;
}

void Core::Core_Statistic::receiveCurrentPerson(Core::Enums::FirstRes status, Core::Person person) {
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::FirstRes>();
    qDebug() << Q_FUNC_INFO << QObject::tr("receive person '%1' with status: '%2'").arg(person.full_name).arg(metaEnum.valueToKey(static_cast<int>(status))) << this;

    switch(status) {
    case Core::Enums::FirstRes::FOUND:
        {
            if(m_sales_report.add(person)) {
                qDebug() << Q_FUNC_INFO << QObject::tr("person '%1' already noted").arg(person.full_name) << this;
                emit showPersonInfo(Core::Enums::SecondRes::FORBIDDEN, person);
            } else {
                qDebug() << Q_FUNC_INFO << QObject::tr("person '%1' noted").arg(person.full_name) << this;
                emit showPersonInfo(Core::Enums::SecondRes::ALLOWED, person);
            }
            break;
        }
    case Core::Enums::FirstRes::NOT_FOUND:
        {
            qDebug() << Q_FUNC_INFO << QObject::tr("person '%1' not found").arg(person.full_name) << this;
            emit showPersonInfo(Core::Enums::SecondRes::NOT_FOUND, {});
            break;
        }
    default: {  }
    }
}

void Core::Core_Statistic::flush_report() {
    end_time = QDateTime::currentDateTime().toString(datetime_format);
    qDebug() << Q_FUNC_INFO << QObject::tr("flush sales report at time from '%1' to '%2'").arg(start_time).arg(end_time) << this;
    auto file = m_sales_report.flush(start_date, start_time, end_time);
    start_time = end_time;
    emit statisticsCreated(file);
}
