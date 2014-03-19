#pragma once

#include "../Shared.h"

class CORE_EXPORT MediaChangedEvent
{
    public:
        explicit MediaChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
