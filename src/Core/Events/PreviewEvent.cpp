#include "PreviewEvent.h"

#include "Global.h"

PreviewEvent::PreviewEvent()
    : QEvent(static_cast<QEvent::Type>(Event::EventType::Preview))
{
}
