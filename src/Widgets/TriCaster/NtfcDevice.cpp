#include "NtfcDevice.h"

#include <QtCore/QStringList>
#include <QtCore/QTimer>

NtfcDevice::NtfcDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      command(NtfcDevice::NONE), port(port), connected(false), address(address)
{
    this->socket = new QTcpSocket(this);

    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

NtfcDevice::~NtfcDevice()
{
}

void NtfcDevice::connectDevice()
{
    if (this->connected)
        return;

    this->socket->connectToHost(this->address, this->port);

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void NtfcDevice::disconnectDevice()
{
    this->socket->blockSignals(true);
    this->socket->disconnectFromHost();
    this->socket->blockSignals(false);

    this->connected = false;
    this->command = NtfcDevice::CONNECTIONSTATE;

    sendNotification();
}

void NtfcDevice::setConnected()
{
    this->connected = true;
    this->command = NtfcDevice::CONNECTIONSTATE;

    sendNotification();
}

void NtfcDevice::setDisconnected()
{
    this->connected = false;
    this->command = NtfcDevice::CONNECTIONSTATE;

    sendNotification();

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

bool NtfcDevice::isConnected() const
{
    return this->connected;
}

int NtfcDevice::getPort() const
{
    return this->port;
}

const QString& NtfcDevice::getAddress() const
{
    return this->address;
}

void NtfcDevice::writeMessage(const QString& message)
{
    if (this->connected)
    {
        QString destination = "UI|shortcuts:response";

        const ushort* destinationData = destination.utf16();
        const ushort* messageData = message.utf16();

        const int destinationDataSize = (destination.length() + 1) * sizeof(ushort);
        const int messageDataSize = (message.length() + 1) * sizeof(ushort);

        // Setup the transission header.
        const TcpMessageHeader tcpMessageHeader = { 1, 0, destinationDataSize, sizeof(MessageHeader) + messageDataSize };

        // Setup the message header.
        const MessageHeader messageHeader = { 2, messageDataSize };

        this->socket->write((char*)&tcpMessageHeader, sizeof(tcpMessageHeader));    // Send tcp header.
        this->socket->write((char*)destinationData, destinationDataSize);           // Send destination.
        this->socket->write((char*)&messageHeader, sizeof(messageHeader));          // Send message header.
        this->socket->write((char*)messageData, messageDataSize);                   // Send message.
        this->socket->flush();
    }
}

void NtfcDevice::resetDevice()
{
    this->command = NtfcDevice::NONE;
}
