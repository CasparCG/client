#pragma once

#include "../Shared.h"

class CORE_EXPORT PresetChangedEvent
{
    public:
        explicit PresetChangedEvent(int presetId = 0);

        int getPresetId() const;

    private:
        int presetId;
};
