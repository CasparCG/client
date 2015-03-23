#include "ExportPresetMenuEvent.h"

#include "Global.h"

ExportPresetMenuEvent::ExportPresetMenuEvent(bool enabled)
    : enabled(enabled)
{
}

bool ExportPresetMenuEvent::getEnabled() const
{
    return this->enabled;
}
