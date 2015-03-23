#include "AllowRemoteTriggeringEvent.h"

#include "Global.h"

AllowRemoteTriggeringEvent::AllowRemoteTriggeringEvent(bool enabled)
    : enabled(enabled)
{
}

bool AllowRemoteTriggeringEvent::getEnabled() const
{
    return this->enabled;
}
