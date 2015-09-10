#include "OscWebSocketListener.h"

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QPair>
#include <QtCore/QDebug>

#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

OscWebSocketListener::OscWebSocketListener(const QString& address, int port, QObject* parent)
    : QObject(parent)
{
    this->socket = new QWebSocketServer("", QWebSocketServer::NonSecureMode, this);
    if (this->socket->listen(QHostAddress::Any, port))
    {
        qDebug("Listening for incoming OSC over WebSocket on port %d", qPrintable(port);

        QObject::connect(&this->socket, SIGNAL(newConnection), this, SLOT(newConnection));
    }
    else
    {
        qWarning("Unable to listen on port %d", port);
    }
}

OscWebSocketListener::~OscWebSocketListener()
{
}

void OscWebSocketListener::start()
{
}

void OscListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint)
{
    char addressBuffer[256];

    endpoint.AddressAsString(addressBuffer);

    QList<QVariant> arguments;
    for (osc::ReceivedMessage::const_iterator iterator = message.ArgumentsBegin(); iterator != message.ArgumentsEnd(); ++iterator)
    {
        const osc::ReceivedMessageArgument& argument = *iterator;

        if (argument.IsBool())
            arguments.push_back(argument.AsBool());
        else if (argument.IsInt32())
            arguments.push_back(QVariant::fromValue<qint32>(argument.AsInt32()));
        else if (argument.IsInt64())
            arguments.push_back(QVariant::fromValue<qint64>(argument.AsInt64()));
        else if (argument.IsFloat())
            arguments.push_back(argument.AsFloat());
        else if (argument.IsDouble())
            arguments.push_back(argument.AsDouble());
        else if (argument.IsString())
            arguments.push_back(argument.AsString());
    }

    QString eventMessage = QString("%1").arg(message.AddressPattern());
    QString eventPath = QString("%1%2").arg(addressBuffer).arg(message.AddressPattern());

    //qDebug("OSC message received: %s", eventPath);

    QMutexLocker locker(&eventsMutex);
    if (eventMessage.startsWith("/control"))
    {
        // Do not overwrite control commands already in queue.
        if (!this->events.contains(eventPath))
            this->events[eventPath] = arguments;
    }
    else
        this->events[eventPath] = arguments;
}

void OscWebSocketListener::sendEventBatch()
{
    QMap<QString, QList<QVariant>> other;
    {
        QMutexLocker locker(&eventsMutex);
        this->events.swap(other);
    }

    foreach (const QString& eventPath, other.keys())
        emit messageReceived(eventPath, other[eventPath]);

    QTimer::singleShot(200, this, SLOT(sendEventBatch()));
}
