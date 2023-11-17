#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QDebug>
#include <QLoggingCategory>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif

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

#ifdef Q_OS_ANDROID
    if(QtAndroid::androidSdkVersion() >= 31) {
        auto result = QtAndroid::checkPermission(QString("android.permission.BLUETOOTH_CONNECT"));
        if(result == QtAndroid::PermissionResult::Denied){
            QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.BLUETOOTH_CONNECT"}));
            if(resultHash["android.permission.BLUETOOTH_CONNECT"] == QtAndroid::PermissionResult::Denied)
                return 0;
        }
    }

    QAndroidJniObject::callStaticMethod<void>("ru/romankartashev/PuppetController/MyActivity", "enableBluetooth");

//    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
//    if ( activity.isValid() )
//    {
//        qDebug() << "activity.isValid()";
//        QAndroidJniObject param = QAndroidJniObject::fromString("android.settings.LOCATION_SOURCE_SETTINGS");

//        if (param.isValid() )
//        {
//            QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", param.object<jstring>());
//            activity.callObjectMethod("startActivity","(Landroid/content/Intent;)V",intent.object<jobject>());
//        }
//    }
#endif

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
