#include "DurationChangedEvent.h"

#include "Global.h"

DurationChangedEvent::DurationChangedEvent(int duration)
    : duration(duration)
{
}

int DurationChangedEvent::getDuration() const
{
    return this->duration;
}
