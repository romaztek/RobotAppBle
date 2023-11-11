#include "btcontroller.h"


BtController::BtController(QObject *parent) : QObject(parent)
{
}

BtController::~BtController()
{
    sendMessageAll(bt_commands.disconnectedCommand());

    if(m_deviceDiscoveryAgent)
        m_deviceDiscoveryAgent->stop();

    foreach(ServiceAndController *sc, servicesAndController) {
        if (sc->m_control->state() != QLowEnergyController::UnconnectedState) {
            sc->m_control->disconnectFromDevice();
            sc->m_control->deleteLater();
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

    readMessagesTimer.setInterval(250);
    connect(&readMessagesTimer, &QTimer::timeout, this, [=]() {
        if(dalnomerState)
            sendMessageToKukla(QString("^") + prevDalnomerValue, QString("Puppet1"));

        for(int index = 0; index < servicesAndController.count(); index++) {
            if(servicesAndController[index]->m_service != nullptr) {
                servicesAndController[index]->m_service->readCharacteristic(servicesAndController[index]->m_readCharacteristic);
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

void BtController::connectToDevices(QList<QString> addresses, QList<QString> names)
{
    qDebug().noquote() << "starting connect to devices:";

    for(int i = 0; i < addresses.size(); i++) {
        qDebug().noquote() << "address:" << addresses[i] << "name: " << names[i];
    }

    m_deviceDiscoveryAgent->stop();

    connected_count_desired = addresses.count();

    connect(this, &BtController::signalToCreateCtl, this, &BtController::createCtl, Qt::QueuedConnection);

    connect(this, &BtController::deviceConnected, [this](QString address, QString name) {
        qDebug().noquote() << "connected device address:" << address << "name: " << name;
        this->connected_count++;
        if(this->connected_count == connected_count_desired) {
            emit fullyConnected();
        }
    });

    for(int i = 0; i < addresses.count(); i++) {
        qDebug().noquote() << "trying to connect to:" << names[i];
        ServiceAndController *newsc = new ServiceAndController();
        newsc->name = names[i];
        newsc->address = addresses[i];

        servicesAndController.append(newsc);

        foreach(auto devInfo, deviceInfos) {
            if(devInfo.address().toString() == addresses[i]) {
                emit signalToCreateCtl(devInfo, addresses[i], names[i]);
            }
        }
    }
}

void BtController::createCtl(QBluetoothDeviceInfo info, QString address, QString name)
{
    QLowEnergyController *m_control = QLowEnergyController::createCentral(info, this);

    sppServicesFoundList.insert(address, false);

    foreach(auto sc, servicesAndController) {
        if(sc->address == address)
            sc->m_control = m_control;
    }

    connect(m_control, &QLowEnergyController::serviceDiscovered,
            [this, address, name](const QBluetoothUuid &gatt){
        serviceDiscovered(gatt, address, name);
    });

    connect(m_control, &QLowEnergyController::discoveryFinished,
            [this, address, name]() {
        serviceScanDone(address, name);
    });

    connect(m_control, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
            this, [this, address, name](QLowEnergyController::Error error) {
        Q_UNUSED(this);
        Q_UNUSED(error);
        emit bleConnectError(name);
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
        if(servicesAndController[index]->m_service != nullptr) {
            servicesAndController[index]->m_service->writeCharacteristic(servicesAndController[index]->m_writeCharacteristic[0],
                    array,
                    servicesAndController[index]->m_writeMode);
        }
    }

    qDebug().noquote() << text;
}

void BtController::setDalnomerState(bool state)
{
    dalnomerState = state;
}

void BtController::sendMessage(QString text, const QList<QString> &addressesArray)
{
    qDebug().noquote() << "send message:" << text << "to:";
    foreach(auto addr, addressesArray) {
        qDebug().noquote() << addr;
    }
    if(servicesAndController.count() == 0)
        return;

//    for(int i = 0; i < addressesArray.size(); i++) {
//        int index = addressesArray.at(i);
//        if(index < 0 || index > servicesAndController.count()-1) {
//            qDebug().noquote() << "Index out of range";
//            return;
//        }
//    }

    foreach(auto address, addressesArray) {
        foreach(auto sc, servicesAndController) {
            if(sc->address == address) {
                qDebug().noquote() << "send to:" << address << ": (" << sc->name << ") :" << text;
                QByteArray text_array = text.toLocal8Bit();

                if(sc->m_service != nullptr) {
                    sc->m_service->writeCharacteristic(sc->m_writeCharacteristic[0],
                            text_array,
                            sc->m_writeMode);
                }
            }
        }
    }
}

void BtController::sendMessageToKukla(QString text, const QString btName)
{
    if(servicesAndController.count() == 0)
        return;

    int index = -1;

    for(int i = 0; i < servicesAndController.size(); i++) {
        if(servicesAndController.at(i)->name == btName)
            index = i;
    }

    if(index == -1) return;

    QByteArray text_array = text.toLocal8Bit();

    if(servicesAndController[index]->m_service != nullptr) {
        servicesAndController[index]->m_service->writeCharacteristic(servicesAndController[index]->m_writeCharacteristic[0],
                text_array,
                servicesAndController[index]->m_writeMode);
    }

    qDebug().noquote() << text;
}

void BtController::searchCharacteristic(ServiceAndController *sc)
{
    if(sc->m_service)
    {
        QList<QLowEnergyCharacteristic> list = sc->m_service->characteristics();

        foreach(QLowEnergyCharacteristic c, list)
        {
            if(c.isValid())
            {
                if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse || c.properties() & QLowEnergyCharacteristic::Write)
                {
                    qDebug().noquote() << "have write permission!";
                    sc->m_writeCharacteristic.append(c);
                    if(c.properties() & QLowEnergyCharacteristic::WriteNoResponse)
                        sc->m_writeMode = QLowEnergyService::WriteWithoutResponse;
                    else
                        sc->m_writeMode = QLowEnergyService::WriteWithResponse;
                }
                if(c.properties() & QLowEnergyCharacteristic::Read)
                {
                    sc->m_readCharacteristic = c;
                }
            }
        }

    }
}

void BtController::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
#if defined Q_OS_IOS || defined Q_OS_MAC
        const QString addr = info.deviceUuid().toString();
        qDebug().noquote() << "Added:" << addr;
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
    qDebug().noquote() << "scanFinished";
}

void BtController::serviceDiscovered(const QBluetoothUuid &gatt, QString address, QString name)
{
    qDebug().noquote() << "found service, gatt:" << gatt << "address:" << address << ", name:" << name;

    if (gatt == QBluetoothUuid(bt_gatt)) {
        qDebug().noquote() << "found serial port:" << gatt.toString();

        foreach(ServiceAndController *sc, servicesAndController) {
            if(sc->address == address) {
                sc->m_service = sc->m_control->createServiceObject(QBluetoothUuid(bt_gatt), nullptr);

                if(sc->m_service != nullptr) {
                    qDebug().noquote() << "FOUND YES";
                    connect(sc->m_service , &QLowEnergyService::stateChanged, [this, address, name](QLowEnergyService::ServiceState s){
                        serviceStateChanged(s, address, name);
                    });
                    sc->m_service->discoverDetails();

                    sppServicesFoundList[address] = true;
                } else {
                    qDebug().noquote() << "SPP Service not found." << "address:" << address << ", name:" << name;
                }
                break;
            }
        }
    }
}

void BtController::serviceScanDone(QString address, QString name)
{
    qDebug().noquote() << "serviceScanDone:" << "address:" << address << ", name: " << name;
    qDebug().noquote() << "gatt found:" << sppServicesFoundList[address];
    if(!sppServicesFoundList[address]) {
        emit bleConnectError(name);
    }
}

void BtController::serviceStateChanged(QLowEnergyService::ServiceState s, QString address, QString name)
{
    return;
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug().noquote() << (tr("Discovering services..."));
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug().noquote() << (tr("Service discovered."));

        ServiceAndController *thisSc;

        foreach(auto sc, servicesAndController) {
            if(sc->address == address) {
                thisSc = sc;
            }
        }

        const QLowEnergyCharacteristic hrChar = thisSc->m_service->characteristic(QBluetoothUuid(QString("0000ffe1-0000-1000-8000-00805f9b34fb")));
        if (!hrChar.isValid()) {
            qDebug().noquote() << ("HR Data not found.");
            break;
        }

        const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_notificationDesc.isValid())
        {
            qDebug() << "YEP";

            searchCharacteristic(thisSc);

            thisSc->m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

            connect(thisSc->m_service, &QLowEnergyService::characteristicRead,
                    this, &BtController::readValueFromService);

//            readMessagesTimer.start();

//            QList<QString> new_addr = {address};

//            sendMessage(bt_commands.connectedCommand(), new_addr);

            emit deviceConnected(address, name);
        }
        break;
    }
    default:
        qDebug().noquote() << s << address;
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

        prevDalnomerValue = value;

        for(int i = 1; i < trimmedMsg.length(); i++) {
            value += trimmedMsg.at(i);
        }


        emit dalnomerValuesGot(trimmedMsg.at(0), value);
    } else {
        if(msg == QString("O") || msg == QString("G") || msg == QString("B")) {
            emit recognitionMsgGot(msg);
        }
    }
}












