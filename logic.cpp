#include "logic.h"

Logic::Logic(QObject *parent) : QObject(parent)
{
    QScreen *screen = qApp->screens().at(0);
    mDensity = screen->devicePixelRatio();
}

Logic::~Logic()
{

}

void Logic::print(QString text) {
    qDebug().noquote() << text;
}

qreal Logic::getDensity() {
    return mDensity;
}

void Logic::init()
{
    qmlRegisterType<Logic>("ru.romankartashev.logic", 1, 0, "Logic");
}

QString Logic::getGamepadName(QVariant deviceId)
{
    //    QList<int> gamepadIDs = QGamepadManager::instance()->connectedGamepads();
    //    qDebug().noquote() << "gamepad ID:" << gamepadIDs.at(0);

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    qDebug().noquote() << "gamepad name:" << QGamepadManager::instance()->gamepadName(deviceId.toInt());
    return QGamepadManager::instance()->gamepadName(deviceId.toInt());
#else
    return QString("");
#endif
}

bool Logic::hasTouchScreen()
{
    QList<const QTouchDevice*> devices = QTouchDevice::devices();

    if(devices.count() == 0)
        return false;
    else {
        for(auto device: devices) {
            if(device->maximumTouchPoints() < 2) {
                return false;
            }
        }
    }

    return true;
}

#ifdef Q_OS_ANDROID
bool Logic::checkException(const char* method, const QAndroidJniObject* obj) {
    static QAndroidJniEnvironment env;
    bool result=false;
    if (env->ExceptionCheck()) {
        qCritical("Exception in %s",method);
        env->ExceptionDescribe();
        env->ExceptionClear();
        result=true;
    }
    if (!(obj==NULL || obj->isValid())) {
        qCritical("Invalid object returned by %s",method);
        result=true;
    }
    return result;
}
#endif
