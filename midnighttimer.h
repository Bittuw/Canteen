#ifndef MIDNIGHTTIMER_H
#define MIDNIGHTTIMER_H

#include <functional>

#include <QObject>
#include <QThread>
#include <QTimer>

#define DAY_MSEC 86400000
#define FOUR_HOUR_MSEC 14400000

// С учетом 1 секунды
#define TO_MIDNIGHT_MSEC(c_msec) DAY_MSEC - 1000 - c_msec
#define TO_FOUR_AFTER_MIDNIGHT(c_msec) TO_MIDNIGHT_MSEC(c_msec) + FOUR_HOUR_MSEC

#define TO_NEXT_MIDNIGHT_SEC(c_sec) TO_MIDNIGHT_MSEC(c_sec * 1000)
#define TO_NEXT_MIDNIGHT_MSEC(c_msec) TO_MIDNIGHT_MSEC(c_msec)

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
        void start(int sec = -1);
        void stop();
    private:
        QTimer timer;
        QThread timer_thread;
    };
}
#endif // MIDNIGHTTIMER_H
