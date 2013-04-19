#include "PresetChangedEvent.h"

#include "Global.h"

PresetChangedEvent::PresetChangedEvent(int presetId)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::PresetChanged)), presetId(presetId)
{
}

int PresetChangedEvent::getPresetId() const
{
    return this->presetId;
}
