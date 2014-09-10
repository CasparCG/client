#include "RepositoryDevice.h"

#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

RepositoryDevice::RepositoryDevice(const QString& address, int port, QObject* parent)
    : RrupDevice(address, port, parent)
{
}

const QString RepositoryDevice::resolveIpAddress() const
{
    if (RrupDevice::getAddress() == "localhost")
        return "127.0.0.1";

    QHostAddress address(RrupDevice::getAddress());
    if (!address.isNull())
        return getAddress(); // The ip address is valid.

    // We don't have a valid ip address. Try to resolve using dns lookup.
    QHostInfo hostInfo = QHostInfo::fromName(RrupDevice::getAddress());
    if (hostInfo.error() != QHostInfo::NoError)
        return "";

    if (hostInfo.addresses().isEmpty())
        return "";

    return hostInfo.addresses().at(0).toString();
}

void RepositoryDevice::subscribe(const QString& rundown, const QString& profile)
{
    writeMessage(QString("SUBSCRIBE %1 %2").arg(rundown).arg(profile));
}

void RepositoryDevice::sendNotification()
{
    switch (RrupDevice::command)
    {
        case RrupDevice::CONNECTIONSTATE:
        {
            emit connectionStateChanged(*this);

            break;
        }
        case RrupDevice::ADD:
        {
            QString storyId = RrupDevice::response.split("\r\n").at(1).trimmed();
            QString data = RrupDevice::response.split("\r\n").at(2).trimmed();

            emit repositoryChanged(RepositoryChangeModel("ADD", storyId, data), *this);

            break;
        }
        case RrupDevice::REMOVE:
        {
            QString storyId = RrupDevice::response.split("\r\n").at(1).trimmed();

            emit repositoryChanged(RepositoryChangeModel("REMOVE", storyId), *this);

            break;
        }
        default:
        {
            break;
        }
    }

    resetDevice();
}
