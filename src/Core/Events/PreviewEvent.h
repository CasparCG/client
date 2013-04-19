#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT PreviewEvent : public QEvent
{
    public:
        explicit PreviewEvent();
};
