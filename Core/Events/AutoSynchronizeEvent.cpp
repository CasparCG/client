#include "AutoSynchronizeEvent.h"

#include "Global.h"

AutoSynchronizeEvent::AutoSynchronizeEvent(bool isAutoSynchronize, int interval)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AutoSynchronize)), interval(interval), isAutoSynchronize(isAutoSynchronize)
{
}

int AutoSynchronizeEvent::getInterval() const
{
    return this->interval;
}

bool AutoSynchronizeEvent::getAutoSynchronize() const
{
    return this->isAutoSynchronize;
}
