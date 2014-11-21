#include "AmcpDevice.h"

#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>

AmcpDevice::AmcpDevice(const QString& address, int port, QObject* parent)
    : QObject(parent),
      command(AmcpDevice::NONE), port(port), state(AmcpDevice::ExpectingHeader), connected(false), address(address), disableCommands(false)
{
    this->socket = new QTcpSocket(this);

    this->decoder = new QTextDecoder(QTextCodec::codecForName("UTF-8"));

    QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    QObject::connect(this->socket, SIGNAL(connected()), this, SLOT(setConnected()));
    QObject::connect(this->socket, SIGNAL(disconnected()), this, SLOT(setDisconnected()));
}

AmcpDevice::~AmcpDevice()
{
    delete this->decoder;
}

void AmcpDevice::connectDevice()
{
    if (this->connected)
        return;

    this->socket->connectToHost(this->address, this->port);

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void AmcpDevice::disconnectDevice()
{
    this->socket->blockSignals(true);
    this->socket->disconnectFromHost();
    this->socket->blockSignals(false);

    this->connected = false;
    this->command = AmcpDevice::CONNECTIONSTATE;

    sendNotification();
}

void AmcpDevice::setConnected()
{
    this->connected = true;
    this->command = AmcpDevice::CONNECTIONSTATE;

    sendNotification();
}

void AmcpDevice::setDisconnected()
{
    this->connected = false;
    this->command = AmcpDevice::CONNECTIONSTATE;

    sendNotification();

    QTimer::singleShot(5000, this, SLOT(connectDevice()));
}

void AmcpDevice::setDisableCommands(bool disable)
{
    this->disableCommands = disable;
}

bool AmcpDevice::isConnected() const
{
    return this->connected;
}

int AmcpDevice::getPort() const
{
    return this->port;
}

const QString& AmcpDevice::getAddress() const
{
    return this->address;
}

void AmcpDevice::writeMessage(const QString& message)
{
    if (this->connected && !this->disableCommands)
    {
        this->socket->write(QString("%1\r\n").arg(message.trimmed()).toUtf8());
        this->socket->flush();
    }
}

void AmcpDevice::readMessage()
{
    while (this->socket->bytesAvailable())
    {
        this->fragments += this->decoder->toUnicode(this->socket->readAll());

        int position;
        while ((position = this->fragments.indexOf("\r\n")) != -1)
        {
            QString line = this->fragments.left(position);
            this->fragments.remove(0, position + 2);

            parseLine(line);
        }
    }
}

AmcpDevice::AmcpDeviceCommand AmcpDevice::translateCommand(const QString& command)
{
    if (command == "LOAD") return AmcpDevice::LOAD;
    else if (command == "LOADBG") return AmcpDevice::LOADBG;
    else if (command == "PLAY") return AmcpDevice::PLAY;
    else if (command == "STOP") return AmcpDevice::STOP;
    else if (command == "CG") return AmcpDevice::CG;
    else if (command == "CLS") return AmcpDevice::CLS;
    else if (command == "CINF") return AmcpDevice::CINF;
    else if (command == "VERSION") return AmcpDevice::VERSION;
    else if (command == "TLS") return AmcpDevice::TLS;
    else if (command == "INFO") return AmcpDevice::INFO;
    else if (command == "INFO SYSTEM") return AmcpDevice::INFOSYSTEM;
    else if (command == "DATA LIST") return AmcpDevice::DATALIST;
    else if (command == "DATA RETRIEVE") return AmcpDevice::DATARETRIEVE;
    else if (command == "CLEAR") return AmcpDevice::CLEAR;
    else if (command == "SET") return AmcpDevice::SET;
    else if (command == "MIXER") return AmcpDevice::MIXER;
    else if (command == "CALL") return AmcpDevice::CALL;
    else if (command == "REMOVE") return AmcpDevice::REMOVE;
    else if (command == "ADD") return AmcpDevice::ADD;
    else if (command == "SWAP") return AmcpDevice::SWAP;
    else if (command == "STATUS") return AmcpDevice::STATUS;
    else if (command == "ERROR") return AmcpDevice::ERROR;
    else if (command == "THUMBNAIL LIST") return AmcpDevice::THUMBNAILLIST;
    else if (command == "THUMBNAIL RETRIEVE") return AmcpDevice::THUMBNAILRETRIEVE;

    return AmcpDevice::NONE;
}

void AmcpDevice::parseLine(const QString& line)
{
    switch (this->state)
    {
        case AmcpDevice::ExpectingHeader:
            parseHeader(line);
            break;
        case AmcpDevice::ExpectingOneline:
            parseOneline(line);
            break;
        case AmcpDevice::ExpectingTwoline:
            parseTwoline(line);
            break;
        case AmcpDevice::ExpectingMultiline:
            parseMultiline(line);
            break;
        default:
            break;
    }
}

void AmcpDevice::parseHeader(const QString& line)
{
    if (line.length() == 0)
        return;

    QStringList tokens = line.split(" ");

    this->code = tokens.at(0).toInt();
    switch (this->code)
    {
        case 200: // The command has been executed and several lines of data are being returned.
            this->state = AmcpDevice::ExpectingMultiline;
            break;
        case 201: // The command has been executed and a line of data is being returned.
        case 400: // Command not understood.
            this->state = AmcpDevice::ExpectingTwoline;
            break;
        default:
            parseOneline(line);
            return;
    }

    this->command = translateCommand(tokens.at(1));
    if (tokens.count() > 3)
        this->command = translateCommand(QString("%1 %2").arg(tokens.at(1)).arg(tokens.at(2)));

    this->response.append(line);
}

void AmcpDevice::parseOneline(const QString& line)
{
    AmcpDevice::response.append(line);

    sendNotification();
}

void AmcpDevice::parseTwoline(const QString& line)
{
    AmcpDevice::response.append(line);

    if (AmcpDevice::response.count() == 2)
        sendNotification();
}

void AmcpDevice::parseMultiline(const QString& line)
{
    if (line.length() == 0)
        sendNotification();
    else
        AmcpDevice::response.append(line);
}

void AmcpDevice::resetDevice()
{
    this->code = 0;
    this->response.clear();
    this->command = AmcpDevice::NONE;
    this->state = AmcpDevice::ExpectingHeader;
}
