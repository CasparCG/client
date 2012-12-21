#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT AutoSynchronizeEvent : public QEvent
{
    public:
        explicit AutoSynchronizeEvent(bool isAutoSynchronize, int interval);

        int getInterval() const;

        bool getAutoSynchronize() const;

    private:
        int interval;
        bool isAutoSynchronize;
};
