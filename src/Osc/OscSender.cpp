#include "OscSender.h"

#include <QtCore/QString>
#include <QtCore/QVariant>

OscSender::OscSender(QObject* parent)
    : QObject(parent)
{
}

OscSender::~OscSender()
{
}

void OscSender::send(const QString& address, int port, const QString& path, const QVariant& message, bool useBundle)
{
    UdpTransmitSocket transmitSocket(IpEndpointName(address.toStdString().c_str(), port));

    char buffer[256];
    osc::OutboundPacketStream stream(buffer, 256);

    bool isInt = false;
    int val = message.toInt(&isInt);

    if (useBundle)
        stream << osc::BeginBundleImmediate;

    if (message.type() == QMetaType::Bool)
        stream << osc::BeginMessage(path.toStdString().c_str()) << message.toBool() << osc::EndMessage;
    else if (message.type() == QMetaType::Double)
        stream << osc::BeginMessage(path.toStdString().c_str()) << message.toDouble() << osc::EndMessage;
    else if (message.type() == QMetaType::Float)
        stream << osc::BeginMessage(path.toStdString().c_str()) << message.toFloat() << osc::EndMessage;
    else if (message.type() == QMetaType::Int)
        stream << osc::BeginMessage(path.toStdString().c_str()) << message.toInt() << osc::EndMessage;
    else if (message.type() == QMetaType::QString)
        stream << osc::BeginMessage(path.toStdString().c_str()) << message.toString().toUtf8().constData() << osc::EndMessage;

    if (useBundle)
        stream << osc::EndBundle;

    transmitSocket.Send(stream.Data(), stream.Size());
}
