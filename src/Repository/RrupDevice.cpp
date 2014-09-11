#include "RrupDevice.h"

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

RrupDevice::RrupDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      command(RrupDevice::NONE), port(port), state(RrupDevice::ExpectingHeader), connected(false), address(address)
{
    this->socket = new QTcpSocket(this);

    this->decoder = new QTextDecoder(QTextCodec::codecForName("UTF-8"));

    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

RrupDevice::~RrupDevice()
{
    delete this->decoder;
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
    while (this->socket->bytesAvailable())
    {
        this->fragments += this->decoder->toUnicode(this->socket->readAll());

        int position;
        while ((position = this->fragments.indexOf("\r\n\r\n")) != -1)
        {
            this->response = this->fragments.left(position);
            this->fragments.remove(0, position + 4);

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
