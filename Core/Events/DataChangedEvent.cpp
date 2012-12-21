#include "DataChangedEvent.h"

#include "Global.h"

DataChangedEvent::DataChangedEvent(int deviceId)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::DataChanged)), deviceId(deviceId)
{
}

int DataChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
