#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT AutoRefreshLibraryEvent : public QEvent
{
    public:
        explicit AutoRefreshLibraryEvent(bool autoRefresh, int interval);

        int getInterval() const;

        bool getAutoRefresh() const;

    private:
        int interval;
        bool autoRefresh;
};
