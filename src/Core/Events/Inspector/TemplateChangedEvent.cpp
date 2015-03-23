#include "TemplateChangedEvent.h"

#include "Global.h"

TemplateChangedEvent::TemplateChangedEvent(int deviceId)
    : deviceId(deviceId)
{
}

int TemplateChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
