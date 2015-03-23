#pragma once

#include "../../Shared.h"

class CORE_EXPORT ReloadRundownMenuEvent
{
    public:
        explicit ReloadRundownMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
