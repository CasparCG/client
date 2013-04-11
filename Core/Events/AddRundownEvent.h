#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT AddRundownEvent : public QEvent
{
    public:
        explicit AddRundownEvent();
};
