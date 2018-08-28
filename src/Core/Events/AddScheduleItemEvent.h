#pragma once

#include "../Shared.h"

#include "Models/ScheduleModel.h"

#include <QtCore/QMetaType>

class CORE_EXPORT AddScheduleItemEvent
{
    public:
        explicit AddScheduleItemEvent(const ScheduleModel& model);
        AddScheduleItemEvent();

        const ScheduleModel& getScheduleModel() const;

    private:
        ScheduleModel model;

};

Q_DECLARE_METATYPE(AddScheduleItemEvent)
