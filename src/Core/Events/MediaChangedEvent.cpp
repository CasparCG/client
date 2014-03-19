#include "MediaChangedEvent.h"

#include "Global.h"

MediaChangedEvent::MediaChangedEvent(int deviceId)
    : deviceId(deviceId)
{
}

int MediaChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
