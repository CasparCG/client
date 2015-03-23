#pragma once

#include "../Shared.h"

class CORE_EXPORT DataChangedEvent
{
    public:
        explicit DataChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
