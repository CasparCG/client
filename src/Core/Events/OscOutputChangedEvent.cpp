#include "OscOutputChangedEvent.h"

#include "Global.h"

OscOutputChangedEvent::OscOutputChangedEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::OscOutputChanged))
{
}
