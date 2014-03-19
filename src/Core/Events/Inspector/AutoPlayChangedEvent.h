#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AutoPlayChangedEvent
{
    public:
        explicit AutoPlayChangedEvent(bool autoPlay);

        bool getAutoPlay() const;

    private:
        bool autoPlay;
};
