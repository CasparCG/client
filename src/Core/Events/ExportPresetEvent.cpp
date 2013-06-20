#include "ExportPresetEvent.h"

#include "Global.h"

ExportPresetEvent::ExportPresetEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::ExportPreset))
{
}
