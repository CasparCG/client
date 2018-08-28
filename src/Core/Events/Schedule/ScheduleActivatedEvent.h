#ifndef SCHEDULEACTIVATEDEVENT_H
#define SCHEDULEACTIVATEDEVENT_H

#include "../../Shared.h"
#include "Models/ScheduleModel.h"

#include <QtCore/QString>

class CORE_EXPORT ScheduleActivatedEvent {
    public:
        explicit ScheduleActivatedEvent(bool enabled, const ScheduleModel& mdl);

        bool isEnabled() const;
        const ScheduleModel& getSchedule() const;

    private:
        bool enabled;
        ScheduleModel model;
};

#endif // SCHEDULEACTIVATEDEVENT_H
