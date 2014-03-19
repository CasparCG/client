#include "DeviceChangedEvent.h"

#include "Global.h"

DeviceChangedEvent::DeviceChangedEvent(const QString& deviceName)
    : deviceName(deviceName)
{
}

const QString& DeviceChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
