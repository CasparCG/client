#include "RemoteRundownTriggeringEvent.h"

#include "Global.h"

RemoteRundownTriggeringEvent::RemoteRundownTriggeringEvent(bool enabled)
    : enabled(enabled)
{
}

bool RemoteRundownTriggeringEvent::getEnabled() const
{
    return this->enabled;
}
