#include "btcontroller.h"

BtController::BtController(QObject *parent) : QObject(parent)
{
}

BtController::~BtController()
{
    sendMessageAll("diss");

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
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(20000);

    connect(m_deviceDiscoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this,
            SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

    m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
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
    QByteArray array = (text + "#").toLocal8Bit();

    for(int index = 0; index < servicesAndController.count(); index++) {
        if(servicesAndController[index].m_service != nullptr) {
            servicesAndController[index].m_service->writeCharacteristic(servicesAndController[index].m_writeCharacteristic[0],
                    array,
                    servicesAndController[index].m_writeMode);
        }
    }
}

void BtController::sendMessage(QString text, int index)
{
    QByteArray array = (text + "#").toLocal8Bit();

    if(servicesAndController[index].m_service != nullptr) {
        servicesAndController[index].m_service->writeCharacteristic(servicesAndController[index].m_writeCharacteristic[0],
                array,
                servicesAndController[index].m_writeMode);
    }
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
        const QString addr = info.address().toString();
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
    if (gatt == QBluetoothUuid(QString("0000ffe0-0000-1000-8000-00805f9b34fb"))) {
        qDebug().noquote() << "found serial port:" << gatt.toString();
        servicesAndController[index].foundSpp = true;

        servicesAndController[index].m_service = servicesAndController[index].m_control->createServiceObject(QBluetoothUuid(QString("0000ffe0-0000-1000-8000-00805f9b34fb")), this);

        if (servicesAndController[index].m_service != nullptr) {
            //disconnect(servicesAndController[index].m_control);

            connect(servicesAndController[index].m_service, &QLowEnergyService::stateChanged, [this, index](QLowEnergyService::ServiceState s){
                serviceStateChanged(s, index);
            });
            //connect(m_service, &QLowEnergyService::characteristicChanged, this, &DeviceHandler::updateHeartRateValue);
            //connect(m_service, &QLowEnergyService::descriptorWritten, this, &DeviceHandler::confirmedDescriptorWrite);
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

            sendMessage(connected_word, index);

            emit deviceConnected(index, servicesAndController[index].name);
        }
        break;
    }
    default:
        //nothing for now
        break;
    }
}












