#include "SwitcherDevice.h"

#include <QtNetwork/QHostAddress>

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QThread>

SwitcherDevice::SwitcherDevice(const QString& address, QObject* parent)
    : QObject(parent),
      command(SwitcherDevice::NONE), connected(false), address(address)
{
    this->atemConnection = new QAtemConnection(this);

    QObject::connect(this->atemConnection, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->atemConnection, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

SwitcherDevice::~SwitcherDevice()
{
}

void SwitcherDevice::connectDevice()
{
    if (this->connected)
        return;

    this->atemConnection->connectToSwitcher(QHostAddress(this->address));

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void SwitcherDevice::disconnectDevice()
{
    this->atemConnection->blockSignals(true);
    this->atemConnection->disconnectFromSwitcher();
    this->atemConnection->blockSignals(false);

    this->connected = false;
    this->command = SwitcherDevice::CONNECTIONSTATE;

    sendNotification();
}

void SwitcherDevice::setConnected()
{
    qDebug() << this->address << QString(": Connected");

    this->connected = true;
    this->command = SwitcherDevice::CONNECTIONSTATE;

    sendNotification();
}

void SwitcherDevice::setDisconnected()
{
    qDebug() << this->address << QString(": Disconnected");

    this->connected = false;
    this->command = SwitcherDevice::CONNECTIONSTATE;

    sendNotification();

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

bool SwitcherDevice::isConnected() const
{
    return this->connected;
}

const QString &SwitcherDevice::getAddress() const
{
    return this->address;
}

void SwitcherDevice::resetDevice()
{
    this->command = SwitcherDevice::NONE;
}
