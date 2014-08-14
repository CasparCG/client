#pragma once

#include "../Shared.h"

class CORE_EXPORT ExportPresetMenuEvent
{
    public:
        explicit ExportPresetMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
