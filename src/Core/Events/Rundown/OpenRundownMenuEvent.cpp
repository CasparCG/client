#include "OpenRundownMenuEvent.h"

#include "Global.h"

OpenRundownMenuEvent::OpenRundownMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool OpenRundownMenuEvent::getEnabled() const
{
    return this->enabled;
}
