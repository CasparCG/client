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

void OscSender::send(const QString& address, int port, const QString& path, const QVariant& message)
{
    UdpTransmitSocket transmitSocket(IpEndpointName(address.toStdString().c_str(), port));

    char buffer[256];
    osc::OutboundPacketStream stream(buffer, 256);

    bool isInt = false;
    int val = message.toInt(&isInt);

    if (message.type() == QMetaType::Bool)
        stream << osc::BeginBundleImmediate << osc::BeginMessage(path.toStdString().c_str()) << message.toBool() << osc::EndMessage << osc::EndBundle;
    else if (message.type() == QMetaType::Double)
        stream << osc::BeginBundleImmediate << osc::BeginMessage(path.toStdString().c_str()) << message.toDouble() << osc::EndMessage << osc::EndBundle;
    else if (message.type() == QMetaType::Float)
        stream << osc::BeginBundleImmediate << osc::BeginMessage(path.toStdString().c_str()) << message.toFloat() << osc::EndMessage << osc::EndBundle;
    else if (message.type() == QMetaType::Int)
        stream << osc::BeginBundleImmediate << osc::BeginMessage(path.toStdString().c_str()) << message.toInt() << osc::EndMessage << osc::EndBundle;   
    else if (message.type() == QMetaType::QString)
        stream << osc::BeginBundleImmediate << osc::BeginMessage(path.toStdString().c_str()) << message.toString().toUtf8().constData() << osc::EndMessage << osc::EndBundle;

    transmitSocket.Send(stream.Data(), stream.Size());
}
