#include "ReloadRundownMenuEvent.h"

#include "Global.h"

ReloadRundownMenuEvent::ReloadRundownMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool ReloadRundownMenuEvent::getEnabled() const
{
    return this->enabled;
}
