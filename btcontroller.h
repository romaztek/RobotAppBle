#ifndef BTCONTROLLER_H
#define BTCONTROLLER_H

#include <QObject>
#include <QStringList>
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
    Q_INVOKABLE void connectToDevices(QList<int> indexes, QList<int> device_indexes);
    Q_INVOKABLE void sendMessage(QString text, int index);
    Q_INVOKABLE void sendMessageAll(QString text);

private:
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QList<Device> devices;
    QList<QBluetoothDeviceInfo> deviceInfos;

    QList<ServiceAndController> servicesAndController;

    const QString connected_word = QString("conn");

    void searchCharacteristic(int index);

    int connected_count = 0;
    int connected_count_desired = 0;

signals:
    void newDeviceFound(QVariant device);
    void foundSpp();
    void fullyConnected();
    void signalToCreateCtl(QBluetoothDeviceInfo info, int index);
    void deviceConnected(int index, QString name);

public slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void createCtl(QBluetoothDeviceInfo info, int index);
    void scanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt, int index);
    void serviceScanDone(int index);
    void serviceStateChanged(QLowEnergyService::ServiceState s, int index);
};

#endif // BTCONTROLLER_H
