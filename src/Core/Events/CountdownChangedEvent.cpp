#include "CountdownChangedEvent.h"

#include "Global.h"

CountdownChangedEvent::CountdownChangedEvent(const QString& countdownTime)
    : countdownTime(countdownTime)
{
}

const QString& CountdownChangedEvent::getCountdownTime() const
{
    return this->countdownTime;
}
