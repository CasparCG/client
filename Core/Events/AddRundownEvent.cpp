#include "AddRundownEvent.h"

#include "Global.h"

AddRundownEvent::AddRundownEvent()
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AddRundown))
{
}
