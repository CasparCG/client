#include "AllowRemoteTriggeringMenuEvent.h"

#include "Global.h"

AllowRemoteTriggeringMenuEvent::AllowRemoteTriggeringMenuEvent(bool enabled)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AllowRemoteTriggeringMenu)), enabled(enabled)
{
}

bool AllowRemoteTriggeringMenuEvent::getEnabled() const
{
    return this->enabled;
}
