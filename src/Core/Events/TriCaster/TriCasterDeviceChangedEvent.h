#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

class CORE_EXPORT TriCasterDeviceChangedEvent
{
    public:
        explicit TriCasterDeviceChangedEvent(const QString& deviceName);

        const QString& getDeviceName() const;

    private:
        QString deviceName;
};
