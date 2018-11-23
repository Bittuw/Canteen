#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QDir>
#include <QSet>

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

        QSet<Core::Person> current_persons; // За весь день

        QString default_path = QDir::currentPath() + QStringLiteral("/statistic/");
        QString flush_report(QString, QString, QString, QString, QSet<Core::Person>&);
    };

    class FtpStatistic : public QObject{
        Q_OBJECT
    public:
        explicit FtpStatistic(QObject* parent = nullptr) : QObject(parent) {}
        void transparent(QSet<Core::Person>, QString);
    };
}
#endif // STATISTIC_H
