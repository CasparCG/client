#include "NewRundownEvent.h"

#include "Global.h"

NewRundownEvent::NewRundownEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::NewRundown))
{
}
