#include "ConnectionStateChangedEvent.h"

#include "Global.h"

ConnectionStateChangedEvent::ConnectionStateChangedEvent(const QString& deviceName, bool connected)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::ConnectionStateChanged)), connected(connected), deviceName(deviceName)
{
}

bool ConnectionStateChangedEvent::getConnected() const
{
    return this->connected;
}

const QString& ConnectionStateChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
