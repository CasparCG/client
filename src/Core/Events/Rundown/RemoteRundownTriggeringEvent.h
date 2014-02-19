#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT RemoteRundownTriggeringEvent : public QEvent
{
    public:
        explicit RemoteRundownTriggeringEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
