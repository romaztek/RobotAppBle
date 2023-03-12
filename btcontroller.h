#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>

class Device
{
public:
    Device(QString _address, QString _name) : address(_address), name(_name) {}
    ~Device() {}
    QString address;
    QString name;
};

class BtController : public QObject
{
    Q_OBJECT
public:
    explicit BtController(QObject *parent = nullptr);
    Q_INVOKABLE void init();
    Q_INVOKABLE QVariant getDevices();
    Q_INVOKABLE void connectTo(int index);
    Q_INVOKABLE void sendMessage(QString text);

private:
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QList<Device> devices;
    QList<QBluetoothDeviceInfo> deviceInfos;
    QLowEnergyController *m_control;
    QLowEnergyService *m_service;
    bool foundSpp = false;
    bool m_foundHeartRateService = false;

    QLowEnergyService::WriteMode m_writeMode;
    QLowEnergyCharacteristic m_readCharacteristic;
    QList<QLowEnergyCharacteristic> m_writeCharacteristic;
    void searchCharacteristic();

signals:
    void returnAddress(QBluetoothDeviceInfo);
    void newDeviceFound(QVariant device);
    void fullyConnected();

public slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void createCtl(QBluetoothDeviceInfo info);
    void scanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState s);
};

#endif // BTCONTROLLER_H
