#pragma once

#include "../../Shared.h"

class CORE_EXPORT AutoRefreshLibraryEvent
{
    public:
        explicit AutoRefreshLibraryEvent(bool autoRefresh, int interval);

        int getInterval() const;

        bool getAutoRefresh() const;

    private:
        int interval;
        bool autoRefresh;
};
