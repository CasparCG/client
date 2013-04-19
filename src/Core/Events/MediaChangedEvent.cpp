#include "MediaChangedEvent.h"

#include "Global.h"

MediaChangedEvent::MediaChangedEvent(int deviceId)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::MediaChanged)), deviceId(deviceId)
{
}

int MediaChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
