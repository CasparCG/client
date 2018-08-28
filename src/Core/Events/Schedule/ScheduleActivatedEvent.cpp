#include "ScheduleActivatedEvent.h"

ScheduleActivatedEvent::ScheduleActivatedEvent(bool enabled, const ScheduleModel &mdl)
    : enabled(enabled), model(mdl)
{

}

bool ScheduleActivatedEvent::isEnabled() const
{
    return this->enabled;
}

const ScheduleModel& ScheduleActivatedEvent::getSchedule() const
{
    return this->model;
}



