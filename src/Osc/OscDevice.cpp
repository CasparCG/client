#include "OscDevice.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

OscDevice::OscDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      address(address), port(port)
{
}

OscDevice::~OscDevice()
{
}

void OscDevice::connectDevice()
{

}

void OscDevice::disconnectDevice()
{
}

const int OscDevice::getPort() const
{
    return this->port;
}

const QString OscDevice::getAddress() const
{
    return this->address;
}
