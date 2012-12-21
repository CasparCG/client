#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT DataChangedEvent : public QEvent
{
    public:
        explicit DataChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
