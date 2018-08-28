#include "ScheduleChangedEvent.h"

#include "Global.h"

ScheduleChangedEvent::ScheduleChangedEvent(int id)
    : id(id)
{

}

int ScheduleChangedEvent::getId() const
{
    return this->id;
}
