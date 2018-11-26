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
    transition_start_time = start_time;

    m_sales_report.reestablish(QStringLiteral("report_") + start_date + QStringLiteral("_abort"));
    //m_sales_report.flush_transition_sales_report(start_date, transition_start_time, end_time);
}

void Core::Core_Statistic::stop() {
    end_time = QDateTime::currentDateTime().toString(datetime_format);
    if(start_date == QDateTime::currentDateTime().toString(date_format))
                m_sales_report.flush_abort(start_date + QStringLiteral("_abort"), start_time, end_time);
    m_sales_report.clear();
    qDebug() << Q_FUNC_INFO << QObject::tr("stop %1").arg(this->metaObject()->className()) << this;
}

void Core::Core_Statistic::receiveCurrentPerson(Core::Enums::FirstRes status, Core::Person person) {
    QMetaEnum metaEnum = QMetaEnum::fromType<Enums::FirstRes>();

    qInfo() << Q_FUNC_INFO << QObject::tr("Receive person '%1' with status: '%2'").arg(person.full_name).arg(metaEnum.valueToKey(static_cast<int>(status))) << this;

    switch(status) {
    case Core::Enums::FirstRes::FOUND:
        {
            auto& temp_per = m_sales_report.add(person);
            temp_per.time = QDateTime::currentDateTime().toString(datetime_format);
            qInfo() << Q_FUNC_INFO << QObject::tr("person '%1' noted").arg(person.full_name) << this;
            emit showPersonInfo(Core::Enums::SecondRes::ALLOWED, person);

//            if(!m_sales_report.add(person)) {
//                qInfo() << Q_FUNC_INFO << QObject::tr("person '%1' already noted").arg(person.full_name) << this;
//                emit showPersonInfo(Core::Enums::SecondRes::FORBIDDEN, person);
//            } else {

            //}
            break;
        }
    case Core::Enums::FirstRes::NOT_FOUND:
        {
            qInfo() << Q_FUNC_INFO << QObject::tr("person '%1' not found").arg(person.full_name) << this;
            emit showPersonInfo(Core::Enums::SecondRes::NOT_FOUND, {});
            break;
        }
    default: {  }
    }
}

void Core::Core_Statistic::flush_report(QString datetime) {\
//    flush_transitional_report(datetime); // force transition

    QDateTime end_time_t = QDateTime::fromString(datetime, datetime_format);
    QDateTime start_time_t{end_time_t.date(), {0, 0, 0}};
    QDate file_name = start_time_t.date();

    end_time = datetime;

    qInfo() << Q_FUNC_INFO <<
               QObject::tr("Flush sales report file '%1.xml' at time from '%2' to '%3'")
                .arg(file_name.toString(date_format))
                .arg(start_time_t.toString(datetime_format)).arg(end_time) << this;

    auto report_file = m_sales_report.flush_sales_report(file_name.toString(date_format), start_time_t.toString(datetime_format), end_time_t.toString(datetime_format));
    auto menu_file = m_sales_report.flush_menu(file_name.toString(date_format), start_time_t.toString(datetime_format), end_time_t.toString(datetime_format));

    start_time = start_time_t.addDays(1).toString(datetime_format); // check
    start_date = start_time_t.addDays(1).toString(date_format);

    m_sales_report.clear();

    emit madeEndDayStatistics(report_file, menu_file);
}

void Core::Core_Statistic::flush_transitional_report(QString datetime) {
    QDateTime end_time_t = QDateTime::fromString(datetime, datetime_format);
    QDateTime start_time_t{end_time_t.date(), {end_time_t.time().hour(), 0, 0}};
    QDate file_name = start_time_t.date();

    end_time = end_time_t.toString(datetime_format);
    transition_start_time = start_time_t.toString(datetime_format);

    qInfo() << Q_FUNC_INFO << QObject::tr("Flush transition sales report file '%1' at time from '%2' to '%3'").arg(file_name.toString(date_format)).arg(transition_start_time).arg(end_time) << this;

    auto report_file = m_sales_report.flush_transition_sales_report(file_name.toString(date_format), transition_start_time, end_time);

    emit madeTransitionStatistics(report_file);
}

void Core::Core_Statistic::set_complex_cost(QString complex_cost) {
    m_sales_report.set_complex(complex_cost.toInt());
    qInfo() << Q_FUNC_INFO << QObject::tr("Set new cost: %1").arg(complex_cost) << this;
}
