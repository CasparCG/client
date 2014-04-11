#include "TriCasterDeviceChangedEvent.h"

#include "Global.h"

TriCasterDeviceChangedEvent::TriCasterDeviceChangedEvent(const QString& deviceName)
    : deviceName(deviceName)
{
}

const QString& TriCasterDeviceChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
