#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQuickStyle>
#include <QQmlContext>
#include <QIcon>

#include "core.h"

int main(int argc, char *argv[])
{
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
    //engine.rootContext()->setContextProperty("TextProvider", core.getTextProvider());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    engine.rootObjects();

    core_thread.start();
    return app.exec();
}
