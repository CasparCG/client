#include "OscListener.h"

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QPair>

OscListener::OscListener(const QString& address, int port, QObject* parent)
    : QObject(parent)
{
    this->socket = new UdpSocket();
    this->socket->SetAllowReuse(true);
    this->socket->Bind(IpEndpointName(address.toStdString().c_str(), port));

    this->multiplexer = new SocketReceiveMultiplexer();
    this->multiplexer->AttachSocketListener(this->socket, this);

    this->thread = new OscThread(this->multiplexer, this);

    this->timer = new QTimer(this);
    this->timer->connect(this->timer, SIGNAL(timeout()), this, SLOT(sendBatch()));
    this->timer->start(100);
}

OscListener::~OscListener()
{
    this->thread->stop();

    delete this->socket;
    delete this->multiplexer;
    delete this->thread;
}

void OscListener::start()
{
    this->thread->start();
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

    QString eventPath = QString("%1%2").arg(addressBuffer).arg(message.AddressPattern());

    QMutexLocker locker(&eventsMutex);
    this->events[eventPath] = arguments;
}

void OscListener::sendBatch()
{
    QMutexLocker locker(&eventsMutex);
    foreach (const QString& eventPath, this->events.keys())
        emit messageReceived(eventPath, this->events[eventPath]);

    this->events.clear();
}
