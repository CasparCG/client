#include "SaveAsPresetEvent.h"

#include "Global.h"

SaveAsPresetEvent::SaveAsPresetEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::SaveAsPreset))
{
}
