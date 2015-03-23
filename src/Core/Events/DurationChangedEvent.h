#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT DurationChangedEvent
{
    public:
        explicit DurationChangedEvent(int duration);

        int getDuration() const;

    private:
        int duration;
};
