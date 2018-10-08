#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QDir>
#include <QSet>

#include "person.h"

namespace Statistic {
    class Statistic : public QObject
    {
        Q_OBJECT
    public:
        explicit Statistic(QObject *parent = nullptr);
        bool add(const Core::Person&); // Поел))
        void flush(); // Сброс
        void reestablish(); // Восстановить текущий день (если есть)
        bool contains(const Core::Person&);
        ~Statistic();

    signals:

    public slots:
    private:
        QSet<Core::Person> next_writeble;
        QString default_path = QDir::currentPath() + "/statistic/";
    };
}
#endif // STATISTIC_H
