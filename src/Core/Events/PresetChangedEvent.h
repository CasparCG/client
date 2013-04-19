#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT PresetChangedEvent : public QEvent
{
    public:
        explicit PresetChangedEvent(int presetId = 0);

        int getPresetId() const;

    private:
        int presetId;
};
