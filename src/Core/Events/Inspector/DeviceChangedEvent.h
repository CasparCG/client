#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT DeviceChangedEvent : public QEvent
{
    public:
        explicit DeviceChangedEvent(const QString& deviceName);

        const QString& getDeviceName() const;

    private:
        QString deviceName;
};
