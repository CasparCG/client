#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT MediaChangedEvent : public QEvent
{
    public:
        explicit MediaChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
