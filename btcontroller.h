#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QDebug>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QTimer>

#include "bt_commands.h"


class Device
{
public:
    Device(QString _address, QString _name) : address(_address), name(_name) {}
    ~Device() {}
    QString address;
    QString name;
};

class ServiceAndController
{
public:
    ServiceAndController(){}
    QLowEnergyController *m_control = nullptr;
    QLowEnergyService *m_service = nullptr;
    QLowEnergyCharacteristic m_readCharacteristic;
    QList<QLowEnergyCharacteristic> m_writeCharacteristic;
    QLowEnergyService::WriteMode m_writeMode;
    bool foundSpp = false;
    QString address;
    QString name;
};

class BtController : public QObject
{
    Q_OBJECT
public:
    explicit BtController(QObject *parent = nullptr);
    ~BtController();
    Q_INVOKABLE void init();
    Q_INVOKABLE QVariant getDevices();
    Q_INVOKABLE void connectToDevices(QList<QString> addresses, QList<QString> names);
    Q_INVOKABLE void sendMessage(QString text, const QList<QString> &addressesArray);
    Q_INVOKABLE void sendMessageToKukla(QString text, const QString btName);
    Q_INVOKABLE void sendMessageAll(QString text);
    Q_INVOKABLE void setDalnomerState(bool state);

private:
    Bt_Commands bt_commands;
    QString bt_gatt = QStringLiteral(u"0000ffe0-0000-1000-8000-00805f9b34fb");
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QList<QBluetoothDeviceInfo> deviceInfos;

    QList<ServiceAndController> servicesAndController;

    QMap<QString, bool> sppServicesFoundList;   // address, found

    void searchCharacteristic(ServiceAndController *sc);

    int connected_count = 0;
    int connected_count_desired = 0;

    QTimer readMessagesTimer;

    bool dalnomerState = false;

    QString prevDalnomerValue = "0";

signals:
    void newDeviceFound(QVariant device);
    void foundSpp();
    void fullyConnected();
    void signalToCreateCtl(QBluetoothDeviceInfo info, QString address, QString name);
    void deviceConnected(QString address, QString name);
    void bleConnectError(QString deviceName);

    void recognitionMsgGot(QString msg);
    void dalnomerValuesGot(QString side, QString value);

public slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void createCtl(QBluetoothDeviceInfo info, QString address, QString name);
    void scanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt, QString address, QString name);
    void serviceScanDone(QString address, QString name);
    void serviceStateChanged(QLowEnergyService::ServiceState s, QString address, QString name);

    void readValueFromService(const QLowEnergyCharacteristic &info, const QByteArray &value);
};

#endif // BTCONTROLLER_H
