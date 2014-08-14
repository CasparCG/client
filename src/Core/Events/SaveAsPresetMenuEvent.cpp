#include "SaveAsPresetMenuEvent.h"

#include "Global.h"

SaveAsPresetMenuEvent::SaveAsPresetMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool SaveAsPresetMenuEvent::getEnabled() const
{
    return this->enabled;
}
