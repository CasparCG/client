#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT SaveAsPresetEvent : public QEvent
{
    public:
        explicit SaveAsPresetEvent();
};
