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
        QString flush(QString, QString, QString); // Сброс (Файл, начало времени, конец времени)
        void reestablish(QString); // Восстановить текущий день (если есть)
        bool contains(const Core::Person&); // Ел?
        QSet<Core::Person> get_full_list;
    private:
        QSet<Core::Person> next_writeble;
        QString default_path = QDir::currentPath() + QStringLiteral("/statistic/");
    };

    class FtpStatistic : public QObject{
        Q_OBJECT
    public:
        explicit FtpStatistic(QObject* parent = nullptr) : QObject(parent) {}
        void transparent(QSet<Core::Person>, QString);
    };
}
#endif // STATISTIC_H
