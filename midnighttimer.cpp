#include "midnighttimer.h"

#include <QDebug>
#include <QTime>

Utils::MidnightTimer::MidnightTimer(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer, &QTimer::timeout, [this](){ emit this->TimeOut(); });
}

void Utils::MidnightTimer::moveToThread(QThread* thread) {
    timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Utils::MidnightTimer::start(int sec) {
//    timer.start(msec);
//    auto is = timer.isActive();
    sec == -1 ?
                timer.start(NEXT_TIMEOUT(QTime::currentTime().msecsSinceStartOfDay())) :
                timer.start(sec*1000);

    qInfo()<< Q_FUNC_INFO << QObject::tr("Start timer: elapsed msecs: '%1'").arg(timer.remainingTime()) << this;
}

void Utils::MidnightTimer::stop() {
    timer.stop();
    qInfo()<< Q_FUNC_INFO << "Timer stoped" << this;
}

