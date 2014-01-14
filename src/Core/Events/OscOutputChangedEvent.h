#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT OscOutputChangedEvent : public QEvent
{
    public:
        explicit OscOutputChangedEvent();
};
