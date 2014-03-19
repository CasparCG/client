#pragma once

#include "../../Shared.h"

class CORE_EXPORT TemplateChangedEvent
{
    public:
        explicit TemplateChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
