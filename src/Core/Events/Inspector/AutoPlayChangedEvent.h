#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT AutoPlayChangedEvent : public QEvent
{
    public:
        explicit AutoPlayChangedEvent(bool autoPlay);

        bool getAutoPlay() const;

    private:
        bool autoPlay;
};
