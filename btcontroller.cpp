#include "btcontroller.h"


BtController::BtController(QObject *parent) : QObject(parent)
{
}

BtController::~BtController()
{
    sendMessageAll(bt_commands.disconnectedCommand());

    if(m_deviceDiscoveryAgent)
        m_deviceDiscoveryAgent->stop();

    /*foreach(QLowEnergyService *m_service, m_services) {
        m_service->deleteLater();
    }*/

    foreach(ServiceAndController sc, servicesAndController) {
        if (sc.m_control->state() != QLowEnergyController::UnconnectedState) {
            sc.m_control->disconnectFromDevice();
            sc.m_control->deleteLater();
        }
    }
}

void BtController::init()
{
#ifdef Q_OS_WINRT
    if(QBluetoothLocalDevice::allDevices().empty())
        return;
#endif

    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(20000);

    connect(m_deviceDiscoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this,
            SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    readMessagesTimer.setInterval(250);
    connect(&readMessagesTimer, &QTimer::timeout, this, [=]() {
        sendMessageToKukla(QString("^") + prevDalnomerValue, QString("Puppet1"));
        for(int index = 0; index < servicesAndController.count(); index++) {
            if(servicesAndController[index].m_service != nullptr) {

                //if(servicesAndController[index].m_service.cha)
                servicesAndController[index].m_service->readCharacteristic(servicesAndController[index].m_readCharacteristic);
            }
        }
    });

}

QVariant BtController::getDevices()
{
    QList<QString> deviceList;
    foreach(QBluetoothDeviceInfo d, deviceInfos) {
        deviceList.append(d.address().toString() + "#" + d.name());
    }
    return (QVariant)deviceList;
}

void BtController::connectToDevices(QList<int> indexes, QList<int> device_indexes)
{    
    m_deviceDiscoveryAgent->stop();

    connected_count_desired = indexes.count();

    connect(this, &BtController::signalToCreateCtl, this, &BtController::createCtl, Qt::QueuedConnection);

    connect(this, &BtController::deviceConnected, [this](int index, QString name) {
        qDebug().noquote() << "connected device at index:" << index << "name: " << name;
        this->connected_count++;
        if(connected_count == connected_count_desired) {
            emit fullyConnected();
        }
    });

    for(int i = 0; i < indexes.count(); i++) {
        qDebug().noquote() << "trying to connect to:" << deviceInfos[indexes[i]].name();
        ServiceAndController newsc;
        newsc.name = deviceInfos[indexes[i]].name();
        servicesAndController.append(newsc);

        emit signalToCreateCtl(deviceInfos[indexes[i]], device_indexes[i]);
    }
}

void BtController::createCtl(QBluetoothDeviceInfo info, int index)
{
    QLowEnergyController *m_control = QLowEnergyController::createCentral(info, this);

    servicesAndController[index].m_control = m_control;

    connect(m_control, &QLowEnergyController::serviceDiscovered,
            [this, index](const QBluetoothUuid &gatt){
        serviceDiscovered(gatt, index);
    });

    connect(m_control, &QLowEnergyController::discoveryFinished,
            [this, index]() {
        serviceScanDone(index);
    });

    connect(m_control, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
            this, [this](QLowEnergyController::Error error) {
        Q_UNUSED(this);
        Q_UNUSED(error);
        qDebug().noquote() << "Cannot connect to remote device."; //Wrong connection
    });

    connect(m_control, &QLowEnergyController::connected, this, [this, m_control]() {
        Q_UNUSED(this);
        qDebug().noquote() << "Controller connected. Search services..."; //Slot function triggered by successful connection
        m_control->discoverServices();
    });

    connect(m_control, &QLowEnergyController::disconnected, this, [this]() {
        Q_UNUSED(this);
        qDebug().noquote() << "LowEnergy controller disconnected"; //Wrong connection
    });

    qDebug().noquote() << "start to connect";

    m_control->connectToDevice();

}

void BtController::sendMessageAll(QString text)
{
    qDebug().noquote() << "send to all:" << text;
    QByteArray array = (text).toLocal8Bit();

    for(int index = 0; index < servicesAndController.count(); index++) {
        if(servicesAndController[index].m_service != nullptr) {
            servicesAndController[index].m_service->writeCharacteristic(servicesAndController[index].m_writeCharacteristic[0],
                    array,
                    servicesAndController[index].m_writeMode);
        }
    }

    qDebug().noquote() << text;
}

void BtController::setDalnomerState(bool state)
{
    dalnomerState = state;
}

void BtController::sendMessage(QString text, const QList<int> &array)
{


    //    for(int i = 0; i < array.size(); i++) {
    //        int index = array.at(i);
    //        qDebug().noquote() << "send to " + QString::number(index) << text;
    //    }

    if(servicesAndController.count() == 0) {
        return;
    }

    for(int i = 0; i < array.size(); i++) {
        int index = array.at(i);
        if(index < 0 || index > servicesAndController.count()-1) {
            qDebug().noquote() << "Index out of range";
            return;
        }
    }

    for(int i = 0; i < array.size(); i++) {
        int index = array.at(i);
        qDebug().noquote() << "send to " + QString::number(index) << ": (" << servicesAndController.at(index).name << ") :" << text;
        QByteArray text_array = text.toLocal8Bit();

        if(servicesAndController[index].m_service != nullptr) {
            servicesAndController[index].m_service->writeCharacteristic(servicesAndController[index].m_writeCharacteristic[0],
                    text_array,
                    servicesAndController[index].m_writeMode);
        }
    }

    qDebug().noquote() << "ANUS PSA" << text;
}

void BtController::sendMessageToKukla(QString text, const QString btName)
{
    if(servicesAndController.count() == 0) {
        return;
    }

    int index = -1;

    for(int i = 0; i < servicesAndController.size(); i++) {
        if(servicesAndController.at(i).name == btName) {
            index = i;
        }
    }

    if(index == -1) return;

    QByteArray text_array = text.toLocal8Bit();

    if(servicesAndController[index].m_service != nullptr) {
        servicesAndController[index].m_service->writeCharacteristic(servicesAndController[index].m_writeCharacteristic[0],
                text_array,
                servicesAndController[index].m_writeMode);
    }

    qDebug().noquote() << text;
}

void BtController::searchCharacteristic(int index)
{
    if(servicesAndController[index].m_service)
    {
        QList<QLowEnergyCharacteristic> list = servicesAndController[index].m_service->characteristics();

        foreach(QLowEnergyCharacteristic c, list)
        {
            if(c.isValid())
            {
                if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse || c.properties() & QLowEnergyCharacteristic::Write)
                {
                    qDebug().noquote() << "have write permission!";
                    servicesAndController[index].m_writeCharacteristic.append(c);
                    if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse)
                        servicesAndController[index].m_writeMode = QLowEnergyService::WriteWithoutResponse;
                    else
                        servicesAndController[index].m_writeMode = QLowEnergyService::WriteWithResponse;
                }
                if(c.properties() & QLowEnergyCharacteristic::Read)
                {
                    servicesAndController[index].m_readCharacteristic = c;
                }
            }
        }

    }

}

void BtController::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
#ifdef Q_OS_IOS
        const QString addr = info.deviceUuid().toString();
#else
        const QString addr = info.address().toString();
#endif
        const QString name = info.name();

        if(!deviceInfos.isEmpty() && deviceInfos.contains(info))
            return;

        deviceInfos.append(info);
        emit newDeviceFound((QVariant)(QString(addr + "#" + name)));
    }
}

void BtController::scanFinished()
{

}

void BtController::serviceDiscovered(const QBluetoothUuid &gatt, int index)
{
    if (gatt == QBluetoothUuid(bt_gatt)) {
        qDebug().noquote() << "found serial port:" << gatt.toString();
        servicesAndController[index].foundSpp = true;

        servicesAndController[index].m_service = servicesAndController[index].m_control->createServiceObject(QBluetoothUuid(bt_gatt), this);

        if (servicesAndController[index].m_service != nullptr) {
            connect(servicesAndController[index].m_service, &QLowEnergyService::stateChanged, [this, index](QLowEnergyService::ServiceState s){
                serviceStateChanged(s, index);
            });
            servicesAndController[index].m_service->discoverDetails();
        } else {
            qDebug().noquote() << "SPP Service not found.";
        }

    }
    //qDebug().noquote() << "gatt:" << gatt.toString();
}

void BtController::serviceScanDone(int index)
{
    if(servicesAndController[index].foundSpp) {

    }

}

void BtController::serviceStateChanged(QLowEnergyService::ServiceState s, int index)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug().noquote() << (tr("Discovering services..."));
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug().noquote() << (tr("Service discovered."));

        const QLowEnergyCharacteristic hrChar = servicesAndController[index].m_service->characteristic(QBluetoothUuid(QString("0000ffe1-0000-1000-8000-00805f9b34fb")));
        if (!hrChar.isValid()) {
            qDebug().noquote() << ("HR Data not found.");
            break;
        }

        const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_notificationDesc.isValid())
        {
            qDebug() << "YEP";

            searchCharacteristic(index);

            servicesAndController[index].m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

            connect(servicesAndController[index].m_service, &QLowEnergyService::characteristicRead, this, &BtController::readValueFromService);
            readMessagesTimer.start();

            QList<int> new_index = {index};

            sendMessage(bt_commands.connectedCommand(), new_index);

            emit deviceConnected(index, servicesAndController[index].name);
        }
        break;
    }
    default:
        //nothing for now
        break;
    }
}

void BtController::readValueFromService(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    Q_UNUSED(info);

    QString msg = QString::fromLocal8Bit(value);

    // if dalnomer is enabled in app
    if(dalnomerState) {
        qDebug().noquote() << "dalnomer:" << msg;
        QString trimmedMsg = msg.trimmed();
        QString value = "";

        for(int i = 1; i < trimmedMsg.length(); i++) {
            value += trimmedMsg.at(i);
        }

        prevDalnomerValue = value;

        emit dalnomerValuesGot(trimmedMsg.at(0), value);
    } else {
        if(msg == QString("O") || msg == QString("G") || msg == QString("B")) {
            emit recognitionMsgGot(msg);
        }
    }
}












