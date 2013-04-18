#include "AddPresetItemEvent.h"

#include "Global.h"

AddPresetItemEvent::AddPresetItemEvent(const QString& preset)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AddPresetItem)), preset(preset)
{
}

const QString& AddPresetItemEvent::getPreset() const
{
    return this->preset;
}
