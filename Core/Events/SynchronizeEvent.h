#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT SynchronizeEvent : public QEvent
{
    public:
        explicit SynchronizeEvent(int delay);

        int getDelay() const;

    private:
        int delay;
};
