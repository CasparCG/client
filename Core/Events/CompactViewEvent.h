#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>

class CORE_EXPORT CompactViewEvent : public QEvent
{
    public:
        explicit CompactViewEvent();
};
