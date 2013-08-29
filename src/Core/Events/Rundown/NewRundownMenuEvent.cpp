#include "NewRundownMenuEvent.h"

#include "Global.h"

NewRundownMenuEvent::NewRundownMenuEvent(bool enabled)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::NewRundownMenu)), enabled(enabled)
{
}

bool NewRundownMenuEvent::getEnabled() const
{
    return this->enabled;
}
