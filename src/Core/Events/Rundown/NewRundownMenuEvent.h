#pragma once

#include "../../Shared.h"

class CORE_EXPORT NewRundownMenuEvent
{
    public:
        explicit NewRundownMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
