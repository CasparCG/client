#include "AMCPDevice.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>

AMCPDevice::AMCPDevice(QObject* parent)
    : QObject(parent), command(AMCPDevice::NONE), state(AMCPDevice::ExpectingHeader), connected(false)
{
    this->socket = new QTcpSocket(this);

    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

AMCPDevice::~AMCPDevice()
{
}

void AMCPDevice::connectDevice(const QString& address, int port)
{
    this->port = port;
    this->address = address;

    this->socket->connectToHost(this->address, this->port);
}

void AMCPDevice::disconnectDevice()
{
    this->socket->disconnectFromHost();
}

void AMCPDevice::reconnectDevice()
{
    if (this->connected)
        return;

    this->connectDevice(this->address, this->port);

    QTimer::singleShot(5000, this, SLOT(reconnectDevice()));
}

void AMCPDevice::setConnected()
{
    this->connected = true;
    this->command = AMCPDevice::CONNECTIONSTATE;
    sendNotification();
}

void AMCPDevice::setDisconnected()
{
    this->connected = false;
    this->command = AMCPDevice::CONNECTIONSTATE;
    sendNotification();

    QTimer::singleShot(5000, this, SLOT(reconnectDevice()));
}

bool AMCPDevice::isConnected() const
{
    return this->connected;
}

int AMCPDevice::getPort() const
{
    return this->port;
}

const QString &AMCPDevice::getAddress() const
{
    return this->address;
}

void AMCPDevice::writeMessage(const QString& message)
{
    if (this->connected)
        this->socket->write(QString("%1\r\n").arg(message.trimmed()).toUtf8());
}

void AMCPDevice::readMessage()
{
    while (this->socket->bytesAvailable() && this->socket->canReadLine())
    {
        this->line += QString::fromUtf8(this->socket->readLine());
        if (this->line.endsWith("\r\n"))
        {
            if (this->line == "\r\n")
                this->line.remove(QRegExp("\r\n"));

            QStringList lines = this->line.split(QRegExp("\r\n"));
            foreach (QString line, lines)
                parseLine(line);

            this->line.clear();
        }
    }
}

AMCPDevice::AMCPCommand AMCPDevice::translateCommand(const QString& command)
{
    if (command == "LOAD") return AMCPDevice::LOAD;
    else if (command == "LOADBG") return AMCPDevice::LOADBG;
    else if (command == "PLAY") return AMCPDevice::PLAY;
    else if (command == "STOP") return AMCPDevice::STOP;
    else if (command == "CG") return AMCPDevice::CG;
    else if (command == "CLS") return AMCPDevice::CLS;
    else if (command == "CINF") return AMCPDevice::CINF;
    else if (command == "VERSION") return AMCPDevice::VERSION;
    else if (command == "TLS") return AMCPDevice::TLS;
    else if (command == "INFO") return AMCPDevice::INFO;
    else if (command == "INFO SYSTEM") return AMCPDevice::INFOSYSTEM;
    else if (command == "DATA LIST") return AMCPDevice::DATALIST;
    else if (command == "CLEAR") return AMCPDevice::CLEAR;
    else if (command == "SET") return AMCPDevice::SET;
    else if (command == "MIXER") return AMCPDevice::MIXER;
    else if (command == "CALL") return AMCPDevice::CALL;
    else if (command == "REMOVE") return AMCPDevice::REMOVE;
    else if (command == "ADD") return AMCPDevice::ADD;
    else if (command == "SWAP") return AMCPDevice::SWAP;
    else if (command == "STATUS") return AMCPDevice::STATUS;

    return AMCPDevice::NONE;
}

void AMCPDevice::parseLine(const QString& line)
{
    switch (this->state)
    {
        case AMCPDevice::ExpectingHeader:
            parseHeader(line);
            break;
        case AMCPDevice::ExpectingOneline:
            parseOneline(line);
            this->previousLine = line;
            break;
        case AMCPDevice::ExpectingTwoline:
            parseTwoline(line);
            this->previousLine = line;
            break;
        case AMCPDevice::ExpectingMultiline:
            parseMultiline(line);
            this->previousLine = line;
            break;
        default:
            break;
    }
}

void AMCPDevice::parseHeader(const QString& line)
{
    if (line.length() == 0)
        return;

    QStringList tokens = line.split(QRegExp("\\s"));

    this->code = tokens.at(0).toInt();
    this->command = translateCommand(tokens.at(1));

    if (tokens.count() == 3)
        this->command = translateCommand(tokens.at(1));
    else
        this->command = translateCommand(QString("%1 %2").arg(tokens.at(1)).arg(tokens.at(2)));

    switch (this->code)
    {
        case 200:
            this->state = AMCPDevice::ExpectingMultiline;
            break;
        case 201:
            this->state = AMCPDevice::ExpectingTwoline;
            break;
        default:
            this->state = AMCPDevice::ExpectingOneline;
            break;
    }

    this->response.append(line);
}

void AMCPDevice::parseOneline(const QString& line)
{
    if (line.length() > 0)
         this->response.append(line);
    else if (line.length() == 0 && this->response.count() > 0)
        sendNotification();
}

void AMCPDevice::parseTwoline(const QString& line)
{
    if (line.length() > 0)
         this->response.append(line);
    else if (line.length() == 0 && this->response.count() > 1)
        sendNotification();
}

void AMCPDevice::parseMultiline(const QString& line)
{
    if (line.length() > 0)
        this->response.append(line);
    else if (line.length() == 0 && this->line.length() == 0 && this->previousLine.length() == 0)
        sendNotification();
}

void AMCPDevice::resetDevice()
{
    this->code = 0;
    this->response.clear();
    this->command = AMCPDevice::NONE;
    this->state = AMCPDevice::ExpectingHeader;
}
