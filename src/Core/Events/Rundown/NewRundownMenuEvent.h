#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT NewRundownMenuEvent : public QEvent
{
    public:
        explicit NewRundownMenuEvent(bool enabled);

        bool getEnabled() const;

    private:
        bool enabled;
};
