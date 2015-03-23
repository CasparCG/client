#pragma once

#include "../../Shared.h"

class CORE_EXPORT AllowRemoteTriggeringEvent
{
    public:
        explicit AllowRemoteTriggeringEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
