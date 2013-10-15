#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT EmptyRundownEvent : public QEvent
{
    public:
        explicit EmptyRundownEvent();
};
