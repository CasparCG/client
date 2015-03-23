#include "AllowRemoteTriggeringMenuEvent.h"

#include "Global.h"

AllowRemoteTriggeringMenuEvent::AllowRemoteTriggeringMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool AllowRemoteTriggeringMenuEvent::getEnabled() const
{
    return this->enabled;
}
