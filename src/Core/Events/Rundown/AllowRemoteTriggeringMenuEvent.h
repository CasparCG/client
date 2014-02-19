#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT AllowRemoteTriggeringMenuEvent : public QEvent
{
    public:
        explicit AllowRemoteTriggeringMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
