#include "core.h"
#include <QThread>
#include <QDebug>

Core::Core::Core(QObject *parent) : QObject(parent)
{

}

void Core::Core::moveToThread(QThread *thread) {
    qDebug() << Q_FUNC_INFO << "starting core." << this;
    QObject::connect(thread, &QThread::start, this, &Core::runCore);
    QObject::connect(thread, &QThread::finished, this, &Core::stop);
    QObject::moveToThread(thread);
}


void Core::Core::runCore() {
    qDebug() << Q_FUNC_INFO << "core started." << this;
    IronLogic::Z2USB reader;
    reader.moveToThread(&m_serial_port);
    m_serial_port.start();
}

void Core::Core::stop() {
    qDebug() << Q_FUNC_INFO << "core stoped." << this;
    m_serial_port.quit();
}
