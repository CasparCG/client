#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT CloseRundownEvent : public QEvent
{
    public:
        explicit CloseRundownEvent();
};
