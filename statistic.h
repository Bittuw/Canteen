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

        void reestablish(QString); // Восстановить текущий день (если есть)
        void clear();
        bool contains(const Core::Person&); // Ел?

    private:
        QSet<Core::Person> current_persons; // За весь день
        QSet<Core::Person> current_persons_h; // Для 1 часа

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
