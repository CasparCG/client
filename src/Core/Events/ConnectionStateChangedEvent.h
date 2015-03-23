#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT ConnectionStateChangedEvent
{
    public:
        explicit ConnectionStateChangedEvent(const QString& deviceName, bool connected);

        bool getConnected() const;
        const QString& getDeviceName() const;

    private:
        bool connected;
        QString deviceName;
};
