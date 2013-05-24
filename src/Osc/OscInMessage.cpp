#include "OscInMessage.h"

OscAddress::OscAddress(const QString& ipAddress, int port)
    : ipAddress(ipAddress), port(port)
{
}

const QString& OscAddress::getIpAddress() const
{
    return ipAddress;
}

int OscAddress::getPort() const
{
    return port;
}

OscInMessage::OscInMessage()
    : origin("")
{
}

OscInMessage::OscInMessage(const OscAddress& origin, const QString& path, const QVector<QVariant>& arguments)
    : origin(origin), path(path), arguments(arguments)
{
}

const OscAddress& OscInMessage::getOrigin() const
{
    return origin;
}

const QString& OscInMessage::getPath() const
{
    return path;
}

const QVector<QVariant>& OscInMessage::getArguments() const
{
    return arguments;
}


