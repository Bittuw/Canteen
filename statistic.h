#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QVector>
#include <QDir>

#include "person.h"

namespace Statistics {

    class SalesReport : public QObject
    {
        Q_OBJECT
    public:
        explicit SalesReport(QObject* parent = nullptr);
        bool add(const Core::Person&); // Поел))

        QString flush_sales_report(QString, QString, QString); // Отчет (Файл, начало времени, конец времени)
        QString flush_abort(QString, QString, QString); // Сброс

        QString flush_menu(QString,QString, QString);
        QString flush_transition_sales_report(QString, QString, QString);

        void set_complex(quint16);

        void reestablish(QString); // Восстановить текущий день (если есть)
        bool contains(const Core::Person&); // Ел?
        Core::Person& get_person(Core::Person&);
        void clear();

    private:
        quint16 m_complex;
        quint16 m_complex_old;

        // Need pass through numbering (Store and Restore)
//        quint64 m_pass_through_numbering = 0; // throught days
//        quint64 m_pass_through_numbering_day = 0; // throught day
//        quint64 m_pass_through_numbering_hour = 0; // throught hour


        QVector<Core::Person> current_persons; // За весь день

        QString default_path = QDir::currentPath() + QStringLiteral("/statistic/");
        QString flush_report(QString, QString, QString, QString, QVector<Core::Person>&);
    };

    class FtpStatistic : public QObject{
        Q_OBJECT
    public:
        explicit FtpStatistic(QObject* parent = nullptr) : QObject(parent) {}
        void transparent(QSet<Core::Person>, QString);
    };
}
#endif // STATISTIC_H
