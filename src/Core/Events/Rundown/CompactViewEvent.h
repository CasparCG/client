#pragma once

#include "../../Shared.h"

class CORE_EXPORT CompactViewEvent
{
    public:
        explicit CompactViewEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
