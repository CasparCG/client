#include "NewRundownMenuEvent.h"

#include "Global.h"

NewRundownMenuEvent::NewRundownMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool NewRundownMenuEvent::getEnabled() const
{
    return this->enabled;
}
