#pragma once

#include "../../Shared.h"

class CORE_EXPORT AllowRemoteTriggeringMenuEvent
{
    public:
        explicit AllowRemoteTriggeringMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
