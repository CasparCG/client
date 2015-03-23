#include "OpenRundownFromUrlMenuEvent.h"

#include "Global.h"

OpenRundownFromUrlMenuEvent::OpenRundownFromUrlMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool OpenRundownFromUrlMenuEvent::getEnabled() const
{
    return this->enabled;
}
