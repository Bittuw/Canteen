#include "midnighttimer.h"

#include <QDebug>
#include <QTime>

Utils::MidnightTimer::MidnightTimer(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer, &QTimer::timeout, [this](){ emit this->TimeOut(QDateTime::currentDateTime()); });
}

void Utils::MidnightTimer::moveToThread(QThread* thread) {
    timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Utils::MidnightTimer::start(int sec) {
    sec == -1 ?
                timer.start(TO_NEXT_MIDNIGHT_MSEC(QTime::currentTime().msecsSinceStartOfDay())) :
                timer.start(sec*1000);

    qInfo()<< Q_FUNC_INFO << QObject::tr("Start timer: elapsed msecs: '%1'").arg(timer.remainingTime()) << this;
}

void Utils::MidnightTimer::stop() {
    timer.stop();
    qInfo()<< Q_FUNC_INFO << "Timer stoped" << this;
}

