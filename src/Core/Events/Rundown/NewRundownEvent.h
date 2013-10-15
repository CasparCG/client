#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT NewRundownEvent : public QEvent
{
    public:
        explicit NewRundownEvent();
};
