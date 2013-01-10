#include "AutoRefreshLibraryEvent.h"

#include "Global.h"

AutoRefreshLibraryEvent::AutoRefreshLibraryEvent(bool isAutoRefresh, int interval)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AutoRefreshLibrary)), interval(interval), isAutoRefresh(isAutoRefresh)
{
}

int AutoRefreshLibraryEvent::getInterval() const
{
    return this->interval;
}

bool AutoRefreshLibraryEvent::getAutoRefresh() const
{
    return this->isAutoRefresh;
}
