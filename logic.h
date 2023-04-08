#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QQmlEngine>
#include <QDebug>
#include <QList>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QTouchDevice>

#include <QBluetoothAddress>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothUuid>

#include <QGamepad>
#include <QGamepadManager>

#include <QStringList>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

#ifdef Q_OS_LINUX
#include <QProcess>
#endif

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = nullptr);
    ~Logic();

    qreal mDensity = 0;

    static void init();

    Q_INVOKABLE void print(QString text);
    Q_INVOKABLE qreal getDensity();

    Q_INVOKABLE QString getGamepadName(QVariant deviceId);
    Q_INVOKABLE bool hasTouchScreen();

#ifdef Q_OS_ANDROID
    bool checkException(const char* method, const QAndroidJniObject* obj);
#endif
 


};

#endif // LOGIC_H
