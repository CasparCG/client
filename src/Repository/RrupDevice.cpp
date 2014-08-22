#include "RrupDevice.h"

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>
 #include <QtCore/QUrl>

RrupDevice::RrupDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      command(RrupDevice::NONE), port(port), state(RrupDevice::ExpectingHeader), connected(false), address(address), profile("")
{
    this->socket = new QTcpSocket(this);

    QUrl url(this->address);
    //QUrl url("http://localhost/urllist/default");

    this->address = url.host();
    this->profile =  QDir(url.path()).dirName();

    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

RrupDevice::~RrupDevice()
{
}

void RrupDevice::connectDevice()
{
    if (this->connected)
        return;

    this->socket->connectToHost(this->address, this->port);

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void RrupDevice::disconnectDevice()
{
    this->socket->blockSignals(true);
    this->socket->disconnectFromHost();
    this->socket->blockSignals(false);

    this->connected = false;
    this->command = RrupDevice::CONNECTIONSTATE;

    sendNotification();
}

void RrupDevice::setConnected()
{
    this->connected = true;
    this->command = RrupDevice::CONNECTIONSTATE;

    sendNotification();
}

void RrupDevice::setDisconnected()
{
    this->connected = false;
    this->command = RrupDevice::CONNECTIONSTATE;

    sendNotification();

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

bool RrupDevice::isConnected() const
{
    return this->connected;
}

int RrupDevice::getPort() const
{
    return this->port;
}

const QString& RrupDevice::getAddress() const
{
    return this->address;
}

const QString& RrupDevice::getProfile() const
{
    return this->profile;
}

void RrupDevice::writeMessage(const QString& message)
{
    if (this->connected)
    {
        this->socket->write(QString("%1\r\n").arg(message.trimmed()).toUtf8());
        this->socket->flush();
    }
}

void RrupDevice::readMessage()
{
    while (this->socket->bytesAvailable() && this->socket->canReadLine())
    {
        this->response += QString::fromUtf8(this->socket->readLine());
        if (this->response.endsWith("\r\n\r\n"))
        {
            QStringList tokens = this->response.split("\r\n");

            this->command = translateCommand(tokens.at(0));

            sendNotification();
        }
    }
}

RrupDevice::RrupDeviceCommand RrupDevice::translateCommand(const QString& command)
{
    if (command == "ADD") return RrupDevice::ADD;
    else if (command == "REMOVE") return RrupDevice::REMOVE;

    return RrupDevice::NONE;
}

void RrupDevice::resetDevice()
{
    this->line.clear();
    this->response.clear();
    this->command = RrupDevice::NONE;
    this->state = RrupDevice::ExpectingHeader;
}
