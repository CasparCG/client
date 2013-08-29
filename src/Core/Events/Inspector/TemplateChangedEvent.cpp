#include "TemplateChangedEvent.h"

#include "Global.h"

TemplateChangedEvent::TemplateChangedEvent(int deviceId)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::TemplateChanged)), deviceId(deviceId)
{
}

int TemplateChangedEvent::getDeviceId() const
{
    return this->deviceId;
}
