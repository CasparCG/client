#pragma once

#include "../../Shared.h"

class CORE_EXPORT SaveAsMenuEvent
{
    public:
        explicit SaveAsMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
