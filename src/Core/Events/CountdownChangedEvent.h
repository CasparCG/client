#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT CountdownChangedEvent
{
    public:
        explicit CountdownChangedEvent(const QString& countdownTime);

        const QString& getCountdownTime() const;

    private:
        QString countdownTime;
};
