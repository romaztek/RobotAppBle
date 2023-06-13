#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QDebug>
#include <QLoggingCategory>

#include "btcontroller.h"
#include "logic.h"
#include "enums.h"
#include "bt_commands.h"

int main(int argc, char *argv[])
{
    qputenv("QT_ANDROID_VOLUME_KEYS", "1"); // "1" is dummy
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = false"));

    QGuiApplication app(argc, argv);

    // ru.romankartashev.PuppetController
    QCoreApplication::setOrganizationName("romankartashev");
    QCoreApplication::setOrganizationDomain("romankartashev.ru");
    QCoreApplication::setApplicationName("PuppetController");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PuppetController_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }


    BtController btController;

    Logic::init();
    Enums::init();
    Bt_Commands::init();


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("btController", &btController);
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
                qDebug().noquote() << "error";
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
