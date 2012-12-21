#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT RundownIsEmptyEvent : public QEvent
{
    public:
        explicit RundownIsEmptyEvent();
};
