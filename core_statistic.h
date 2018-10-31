#ifndef CORE_STATISTIC_H
#define CORE_STATISTIC_H

#include <QObject>
#include <QString>
#include <QSet>

#include "statistic.h"
#include "person.h"

namespace Core {
    class Core_Statistic : public QObject
    {
        Q_OBJECT
    public:
        explicit Core_Statistic(QObject* parent = nullptr);
        void moveToThread(QThread*);

    signals:
        void showPersonInfo(Core::Enums::SecondRes, Core::Person); // Status, Person
        void madeEndDayStatistics(QString, QString = ""); // sales_report, menu (possible) end_day
        void madeTransitionStatistics(QString, QString = ""); // sales_report, menu (possible) transition

    public slots:
        void start();
        void stop();
        void receiveCurrentPerson(Core::Enums::FirstRes, Core::Person); // from device

        void flush_report(QString); //
        void flush_transitional_report(QString); //

    private:
        Statistics::SalesReport m_sales_report;

        QString date_format = QStringLiteral("yyyyMMdd");
        QString datetime_format = QStringLiteral("yyyy-MM-dd hh:mm:ss");

        QString start_date;
        QString start_time;
        QString end_time;

        QString transition_start_time;

        Q_DISABLE_COPY(Core_Statistic)
    };
}
#endif // CORE_STATISTIC_H
