#include "DataChangedEvent.h"

#include "Global.h"

DataChangedEvent::DataChangedEvent(int deviceId)
    : deviceId(deviceId)
{
}

int DataChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
