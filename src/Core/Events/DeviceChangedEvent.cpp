#include "DeviceChangedEvent.h"

#include "Global.h"

DeviceChangedEvent::DeviceChangedEvent(const QString& deviceName)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::DeviceChanged)), deviceName(deviceName)
{
}

const QString& DeviceChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
