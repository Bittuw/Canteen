#include "midnighttimer.h"

#include <QTime>

Utils::MidnightTimer::MidnightTimer(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer, &QTimer::timeout, [this](){ emit this->TimeOut(); });
}

void Utils::MidnightTimer::moveToThread(QThread* thread) {
    timer.moveToThread(thread);
    QObject::moveToThread(thread);
}

void Utils::MidnightTimer::start() {
   emit timer.start(NEXT_TIMEOUT(QTime::currentTime().msec()));
}

void Utils::MidnightTimer::stop() {
    emit timer.stop();
}

