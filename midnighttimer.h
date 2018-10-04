#ifndef MIDNIGHTTIMER_H
#define MIDNIGHTTIMER_H

#include <functional>

#include <QObject>
#include <QThread>
#include <QTimer>

#define toMidnight(sec) 84600000 - sec

namespace Utils {
    class MidnightTimer : public QObject
    {
        Q_OBJECT
    public:
        explicit MidnightTimer(QObject *parent = nullptr);

    signals:
        void TimeOut();
    public slots:
        void start();
        void stop();
    private:
        QTimer timer;
        QThread timer_thread;
    };
}
#endif // MIDNIGHTTIMER_H
