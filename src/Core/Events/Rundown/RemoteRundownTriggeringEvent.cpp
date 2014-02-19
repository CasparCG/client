#include "RemoteRundownTriggeringEvent.h"

#include "Global.h"

RemoteRundownTriggeringEvent::RemoteRundownTriggeringEvent(bool enabled)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::RemoteRundownTriggering)), enabled(enabled)
{
}

bool RemoteRundownTriggeringEvent::getEnabled() const
{
    return this->enabled;
}
