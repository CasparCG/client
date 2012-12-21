#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT RundownItemPreviewEvent : public QEvent
{
    public:
        explicit RundownItemPreviewEvent();
};
