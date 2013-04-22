#include "AutoRefreshLibraryEvent.h"

#include "Global.h"

AutoRefreshLibraryEvent::AutoRefreshLibraryEvent(bool autoRefresh, int interval)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AutoRefreshLibrary)), interval(interval), autoRefresh(autoRefresh)
{
}

int AutoRefreshLibraryEvent::getInterval() const
{
    return this->interval;
}

bool AutoRefreshLibraryEvent::getAutoRefresh() const
{
    return this->autoRefresh;
}
