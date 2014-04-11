#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

class CORE_EXPORT AtemDeviceChangedEvent
{
    public:
        explicit AtemDeviceChangedEvent(const QString& deviceName);

        const QString& getDeviceName() const;

    private:
        QString deviceName;
};
