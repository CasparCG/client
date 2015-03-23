#include "AutoRefreshLibraryEvent.h"

#include "Global.h"

AutoRefreshLibraryEvent::AutoRefreshLibraryEvent(bool autoRefresh, int interval)
    : interval(interval), autoRefresh(autoRefresh)
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
