#include "AtemDeviceChangedEvent.h"

#include "Global.h"

AtemDeviceChangedEvent::AtemDeviceChangedEvent(const QString& deviceName)
    : deviceName(deviceName)
{
}

const QString& AtemDeviceChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
