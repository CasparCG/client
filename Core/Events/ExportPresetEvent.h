#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT ExportPresetEvent : public QEvent
{
    public:
        explicit ExportPresetEvent();
};
