#include "btcontroller.h"

BtController::BtController(QObject *parent) : QObject(parent)
{
}

void BtController::init()
{
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(20000);

    connect(m_deviceDiscoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this,
            SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
    connect(this, SIGNAL(returnAddress(QBluetoothDeviceInfo)), this, SLOT(createCtl(QBluetoothDeviceInfo)));

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

void BtController::connectTo(int index)
{
    qDebug().noquote() << "trying to connect to:" << deviceInfos[index].name();
    createCtl(deviceInfos[index]);
}

void BtController::sendMessage(QString text)
{
    QByteArray array = text.toLocal8Bit();

    m_service->writeCharacteristic(m_writeCharacteristic[0], array, m_writeMode);
}

void BtController::searchCharacteristic()
{
    if(m_service)
    {
        QList<QLowEnergyCharacteristic> list = m_service->characteristics();

        foreach(QLowEnergyCharacteristic c, list)
        {
            if(c.isValid())
            {
                if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse || c.properties() & QLowEnergyCharacteristic::Write)
                {
                    qDebug().noquote() << "have write permission!\n";
                    m_writeCharacteristic.append(c);
                    if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse)
                        m_writeMode = QLowEnergyService::WriteWithoutResponse;
                    else
                        m_writeMode = QLowEnergyService::WriteWithResponse;
                }
                if(c.properties() & QLowEnergyCharacteristic::Read)
                {
                    m_readCharacteristic = c;
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

        foreach(QString s, oui_prefixes) {
            if(!addr.startsWith(s))
                return;
        }

        if(!deviceInfos.isEmpty() && deviceInfos.contains(info))
            return;

        deviceInfos.append(info);
        emit newDeviceFound((QVariant)(QString(addr + "#" + name)));
    }
}

void BtController::createCtl(QBluetoothDeviceInfo info)
{
    m_control = QLowEnergyController::createCentral(info, this);
    connect(m_control, &QLowEnergyController::serviceDiscovered,
            this, &BtController::serviceDiscovered); //The operation triggered after the target device is found

    connect(m_control, &QLowEnergyController::discoveryFinished,
            this, &BtController::serviceScanDone); //Configure the service

    connect(m_control, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
            this, [this](QLowEnergyController::Error error) {
        Q_UNUSED(error);
        qDebug().noquote() << "Cannot connect to remote device."; //Wrong connection
    });

    connect(m_control, &QLowEnergyController::connected, this, [this]() {
        qDebug().noquote() << "Controller connected. Search services...\n"; //Slot function triggered by successful connection
        m_control->discoverServices();
        //isconnected=true;
    });

    connect(m_control, &QLowEnergyController::disconnected, this, [this]() {
        qDebug().noquote() << "LowEnergy controller disconnected"; //Wrong connection
    });

    //connect
    qDebug().noquote() << "start to connect\n";

    m_control->connectToDevice(); //Start connecting to the target device

}

void BtController::scanFinished()
{

}

void BtController::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if (gatt == QBluetoothUuid(QString("0000ffe0-0000-1000-8000-00805f9b34fb"))) {
        qDebug().noquote() << "found serial port:" << gatt.toString();
        foundSpp = true;
    }


    qDebug().noquote() << "gatt:" << gatt.toString();
}

void BtController::serviceScanDone()
{
    if(foundSpp) {
        m_service = m_control->createServiceObject(QBluetoothUuid(QString("0000ffe0-0000-1000-8000-00805f9b34fb")), this);
    }

    if (m_service) {
        connect(m_service, &QLowEnergyService::stateChanged, this, &BtController::serviceStateChanged);
        //connect(m_service, &QLowEnergyService::characteristicChanged, this, &DeviceHandler::updateHeartRateValue);
        //connect(m_service, &QLowEnergyService::descriptorWritten, this, &DeviceHandler::confirmedDescriptorWrite);
        m_service->discoverDetails();
    } else {
        qDebug().noquote() << "SPP Service not found.";
    }

}

void BtController::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug().noquote() << (tr("Discovering services..."));
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug().noquote() << (tr("Service discovered."));

        const QLowEnergyCharacteristic hrChar = m_service->characteristic(QBluetoothUuid(QString("0000ffe1-0000-1000-8000-00805f9b34fb")));
        if (!hrChar.isValid()) {
            qDebug().noquote() << ("HR Data not found.");
            break;
        }

        const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_notificationDesc.isValid())
        {
            qDebug() << "YEP";

            searchCharacteristic();

            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

            sendMessage("Connected");

            emit fullyConnected();
        }


        //        m_notificationDesc = hrChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
        //        if (m_notificationDesc.isValid())
        //            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

        break;
    }
    default:
        //nothing for now
        break;
    }
}












