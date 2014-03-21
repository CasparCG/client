#pragma once

#include "../../Shared.h"

class CORE_EXPORT RefreshLibraryEvent
{
    public:
        explicit RefreshLibraryEvent(int delay = 0);

        int getDelay() const;

    private:
        int delay;
};
