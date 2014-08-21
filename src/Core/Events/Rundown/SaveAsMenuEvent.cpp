#include "SaveAsMenuEvent.h"

#include "Global.h"

SaveAsMenuEvent::SaveAsMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool SaveAsMenuEvent::getEnabled() const
{
    return this->enabled;
}
