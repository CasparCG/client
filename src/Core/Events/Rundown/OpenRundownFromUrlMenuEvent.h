#pragma once

#include "../../Shared.h"

class CORE_EXPORT OpenRundownFromUrlMenuEvent
{
    public:
        explicit OpenRundownFromUrlMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
