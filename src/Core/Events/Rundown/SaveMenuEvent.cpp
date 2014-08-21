#include "SaveMenuEvent.h"

#include "Global.h"

SaveMenuEvent::SaveMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool SaveMenuEvent::getEnabled() const
{
    return this->enabled;
}
