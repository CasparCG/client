#pragma once

#include "../../Shared.h"

class CORE_EXPORT SaveMenuEvent
{
    public:
        explicit SaveMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
