#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQuickStyle>
#include <QQmlContext>
#include <QDateTime>
#include <QFile>
#include <QIcon>

#include "core.h"

void myMessageHandler(QtMsgType type, QMessageLogContext &context, const QString& msg) {
    Q_UNUSED(context)

    QByteArray localMsg = msg.toLocal8Bit();

    switch (type) {
    case QtDebugMsg: {
        QString message;
        message = QString("DEBUG (%2): %1").arg(localMsg.constData()).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss"));
        QFile log("DEBUG");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&log);
        ts << message << endl;
        log.close();
        break;
    }
    case QtInfoMsg:{
        QString message;
        message = QString("INFO (%2): %1").arg(localMsg.constData()).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss"));
        QFile log("INFO");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&log);
        ts << message << endl;
        log.close();
        break;
    }
    case QtWarningMsg:{
        QString message;
        message = QString("WARNING (%2): %1").arg(localMsg.constData()).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss"));
        QFile log("WARNING");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&log);
        ts << message << endl;
        log.close();
        break;
    }
    case QtCriticalMsg:{
        QString message;
        message = QString("CRITICAL (%2): %1").arg(localMsg.constData()).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss"));
        QFile log("CRITICAL");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&log);
        ts << message << endl;
        log.close();
        break;
    }
    case QtFatalMsg:{
        QString message;
        message = QString("FATAL (%2): %1").arg(localMsg.constData()).arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss"));
        QFile log("FATAL");
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&log);
        ts << message << endl;
        break;
//        abort();
    }
    }

}

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(reinterpret_cast<QtMessageHandler>(myMessageHandler));
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QIcon icon(QStringLiteral(":/icons/favicon.ico"));
    app.setWindowIcon(icon);

    QQuickStyle::setStyle("Material");

    Core::Core core;
    QThread core_thread;
    QObject::connect(&app, &QGuiApplication::aboutToQuit, &core_thread, &QThread::quit);
    core.moveToThread(&core_thread);

    qmlRegisterType<Images::ImageItem>("com.bittumworker.org", 1, 0, "ImageItem");

    ///
    /// implement Core::Core here
    ///

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("DIProvider", core.getProvider());
    engine.rootContext()->setContextProperty("TextProvider", core.getTextProvider());
    engine.rootContext()->setContextProperty("TextProviderDate", core.getTextProviderStatus());
    engine.rootContext()->setContextProperty("TextProviderDateTime", core.getTextProviderDateTime());
    engine.rootContext()->setContextProperty("ComplexProvider", core.getComplexProvider());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    engine.rootObjects();

    core_thread.start();
    return app.exec();
}
