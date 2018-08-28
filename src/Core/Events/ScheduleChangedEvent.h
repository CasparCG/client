#pragma once

#include "../Shared.h"

class CORE_EXPORT ScheduleChangedEvent
{
    public:
        explicit ScheduleChangedEvent(int id = 0);

        int getId() const;

    private:

        int id;
};

