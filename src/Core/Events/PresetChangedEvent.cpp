#include "PresetChangedEvent.h"

#include "Global.h"

PresetChangedEvent::PresetChangedEvent(int presetId)
    : presetId(presetId)
{
}

int PresetChangedEvent::getPresetId() const
{
    return this->presetId;
}
