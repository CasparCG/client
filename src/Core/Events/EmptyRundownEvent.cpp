#include "EmptyRundownEvent.h"

#include "Global.h"

EmptyRundownEvent::EmptyRundownEvent()
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::EmptyRundown))
{
}
