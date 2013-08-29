#include "OpenRundownEvent.h"

#include "Global.h"

OpenRundownEvent::OpenRundownEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::OpenRundown))
{
}
