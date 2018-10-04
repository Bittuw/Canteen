#include "midnighttimer.h"

#include <QTime>

Utils::MidnightTimer::MidnightTimer(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer, &QTimer::timeout, [this](){ emit this->TimeOut(); });
}

void Utils::MidnightTimer::start() {
    timer.start(toMidnight(QTime::currentTime().second()));
//    timer.start(10000);
}

void Utils::MidnightTimer::stop() {
    timer.stop();
}

