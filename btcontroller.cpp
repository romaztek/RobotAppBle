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
    foreach(Device d, devices) {
        deviceList.append(d.address + "#" + d.name);
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

        qDebug()<<"list.count()=" << list.count();
        //characteristics for detailed characteristics
        //SendMaxMode=list.count(); //Set the upper limit of mode selection
        for(int i=0;i<list.count();i++)
        {
            QLowEnergyCharacteristic c = list.at(i);
            /* If the QLowEnergyCharacteristic object is valid, return true, otherwise return false*/
            if(c.isValid())
            {
                // Return the attribute of the feature.
                // These attributes define the access rights of the feature.
                if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse || c.properties() & QLowEnergyCharacteristic::Write)
                    // if(c.properties() & QLowEnergyCharacteristic::Write)
                {
                    qDebug().noquote() << "have write permission!\n";
                    //m_writeCharacteristic[i] = c; //Save write permission feature
                    m_writeCharacteristic.append(c);
                    if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse)
                        // If using this mode write feature, the remote peripheral should not send a write confirmation.
                        // The success of the operation cannot be determined, and the payload must not exceed 20 bytes.
                        // A feature must set the QLowEnergyCharacteristic :: WriteNoResponse property to support this write mode.
                        // Its advantage is a faster write operation, because it may occur between other device interactions.
                        m_writeMode = QLowEnergyService::WriteWithoutResponse;
                    else
                        m_writeMode = QLowEnergyService::WriteWithResponse;
                    //If you use this mode to write features, the peripheral should send a write confirmation.
                    //If the operation is successful, a confirmation is sent through the characteristicWritten() signal.
                    // Otherwise, CharacteristicWriteError is issued.
                    //A feature must set the QLowEnergyCharacteristic :: Write property to support this write mode.
                }
                if(c.properties() & QLowEnergyCharacteristic::Read)
                {
                    m_readCharacteristic = c; //Save read permission feature
                }
                //Descriptor defines how the feature is configured by a specific client.
                //m_notificationDesc = c.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
                //The value is true
//                if(m_notificationDesc.isValid())
//                {
//                    //Write descriptor
//                    m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
//                    //   m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("FEE1"));
//                    qDebug().noquote() << "write descriptor!\n";
//                }
            }
        }
    }

}

void BtController::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {//Determine whether it is a BLE device
        if(devices.isEmpty()) {
            Device newDevice(info.address().toString(), info.name());
            devices.append(newDevice);
            deviceInfos.append(info);
            emit newDeviceFound((QVariant)(QString(newDevice.address + "#" + newDevice.name)));
        } else {
            bool exist = false;
            foreach(Device d, devices) {
                if(d.address == info.address().toString() && d.name == info.name()) {
                    exist = true;
                }
            }
            if(!exist) {
                Device newDevice(info.address().toString(), info.name());
                devices.append(newDevice);
                deviceInfos.append(info);
                emit newDeviceFound((QVariant)(QString(newDevice.address + "#" + newDevice.name)));
            }
        }
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
        //setInfo("Heart Rate service discovered. Waiting for service scan to be done...");
        //m_foundHeartRateService = true;
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












