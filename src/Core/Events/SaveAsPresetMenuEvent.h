#pragma once

#include "../Shared.h"

class CORE_EXPORT SaveAsPresetMenuEvent
{
    public:
        explicit SaveAsPresetMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
