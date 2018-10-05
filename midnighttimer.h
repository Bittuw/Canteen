#ifndef MIDNIGHTTIMER_H
#define MIDNIGHTTIMER_H

#include <functional>

#include <QObject>
#include <QThread>
#include <QTimer>

#define FOUR_HOUR 14400000
#define TO_MIDNIGHT(msec) 86400000 - msec
#define NEXT_TIMEOUT(c_msec) TO_MIDNIGHT(c_msec) + FOUR_HOUR

namespace Utils {
    class MidnightTimer : public QObject
    {
        Q_OBJECT
    public:
        explicit MidnightTimer(QObject *parent = nullptr);
        void moveToThread(QThread*);
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
