#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT ImportPresetEvent : public QEvent
{
    public:
        explicit ImportPresetEvent();
};
