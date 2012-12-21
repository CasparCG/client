#include "SynchronizeEvent.h"

#include "Global.h"

SynchronizeEvent::SynchronizeEvent(int delay)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::Synchronize)), delay(delay)
{
}

int SynchronizeEvent::getDelay() const
{
    return this->delay;
}
