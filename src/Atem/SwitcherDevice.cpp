#include "SwitcherDevice.h"

#include "qatemconnection.h"

#include <QtNetwork/QHostAddress>

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QThread>

SwitcherDevice::SwitcherDevice(const QString& address, QObject* parent)
    : QObject(parent), address(address)
{
    this->atemConnection = new QAtemConnection(this);

    QObject::connect(this->atemConnection, &QAtemConnection::connected, this, &SwitcherDevice::setConnected);
    QObject::connect(this->atemConnection, &QAtemConnection::disconnected, this, &SwitcherDevice::setDisconnected);
}

SwitcherDevice::~SwitcherDevice()
{
}

void SwitcherDevice::connectDevice()
{
    if (this->connected)
        return;

    this->atemConnection->connectToSwitcher(QHostAddress(this->address));
}

void SwitcherDevice::disconnectDevice()
{
    this->atemConnection->blockSignals(true);
    this->atemConnection->disconnectFromSwitcher();
    this->atemConnection->blockSignals(false);

    this->connected = false;
    this->command = SwitcherDeviceCommand::CONNECTIONSTATE;

    sendNotification();
}

void SwitcherDevice::setConnected()
{
    this->connected = true;
    this->command = SwitcherDeviceCommand::CONNECTIONSTATE;

    sendNotification();
}

void SwitcherDevice::setDisconnected()
{
    this->connected = false;
    this->command = SwitcherDeviceCommand::CONNECTIONSTATE;

    sendNotification();

    QTimer::singleShot(5000, this, &SwitcherDevice::connectDevice);
}

bool SwitcherDevice::isConnected() const
{
    return this->connected;
}

const QString& SwitcherDevice::getAddress() const
{
    return this->address;
}

void SwitcherDevice::resetDevice()
{
    this->command = SwitcherDeviceCommand::NONE;
}
