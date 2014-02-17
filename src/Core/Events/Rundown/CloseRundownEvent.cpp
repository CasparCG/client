#include "CloseRundownEvent.h"

#include "Global.h"

CloseRundownEvent::CloseRundownEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::CloseRundown))
{
}
