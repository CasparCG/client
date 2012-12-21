#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT ConnectionStateChangedEvent : public QEvent
{
    public:
        explicit ConnectionStateChangedEvent(const QString& deviceName, bool connected);

        bool getConnected() const;
        const QString& getDeviceName() const;

    private:
        bool connected;
        QString deviceName;
};
