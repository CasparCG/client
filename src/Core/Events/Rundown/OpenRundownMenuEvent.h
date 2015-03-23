#pragma once

#include "../../Shared.h"

class CORE_EXPORT OpenRundownMenuEvent
{
    public:
        explicit OpenRundownMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
