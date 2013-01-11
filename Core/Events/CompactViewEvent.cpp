#include "CompactViewEvent.h"

#include "Global.h"

CompactViewEvent::CompactViewEvent()
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::ToggleCompactView))
{
}
