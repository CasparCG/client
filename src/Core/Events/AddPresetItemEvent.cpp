#include "AddPresetItemEvent.h"

#include "Global.h"

AddPresetItemEvent::AddPresetItemEvent(const QString& preset)
    : preset(preset)
{
}

const QString& AddPresetItemEvent::getPreset() const
{
    return this->preset;
}
