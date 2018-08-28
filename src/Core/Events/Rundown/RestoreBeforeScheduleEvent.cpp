#include "RestoreBeforeScheduleEvent.h"

RestoreBeforeScheduleEvent::RestoreBeforeScheduleEvent(int id)
    : id(id)
{
}

int RestoreBeforeScheduleEvent::getId() const
{
    return this->id;
}

