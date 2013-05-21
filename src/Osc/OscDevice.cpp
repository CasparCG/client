#include "OscDevice.h"

OscDevice::OscDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      address(address), port(port)
{
    this->listener = new OscListener(this);
    this->socket = new UdpListeningReceiveSocket(IpEndpointName(this->address.toStdString().c_str(), this->port), this->listener);
}

OscDevice::~OscDevice()
{
    if (this->socket != NULL)
        delete this->socket;

    if (this->listener != NULL)
        delete this->listener;
}

void OscDevice::connectDevice()
{
    this->socket->Break();
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
