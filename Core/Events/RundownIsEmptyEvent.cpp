#include "RundownIsEmptyEvent.h"

#include "Global.h"

RundownIsEmptyEvent::RundownIsEmptyEvent()
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RundownIsEmpty))
{
}
