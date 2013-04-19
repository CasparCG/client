#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT TemplateChangedEvent : public QEvent
{
    public:
        explicit TemplateChangedEvent(int deviceId = 0);

        int getDeviceId() const;

    private:
        int deviceId;
};
