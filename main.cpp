#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "btcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = false"));

    QGuiApplication app(argc, argv);

    BtController btController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("btController", &btController);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
