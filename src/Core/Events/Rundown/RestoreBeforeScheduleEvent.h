#pragma once

#include "../../Shared.h"

#include <QtCore/QMetaType>

class CORE_EXPORT RestoreBeforeScheduleEvent
{
    public:
        explicit RestoreBeforeScheduleEvent(int id = 0);

        int getId() const;

    private:
        int id;

};

Q_DECLARE_METATYPE(RestoreBeforeScheduleEvent)
