#include "AddScheduleItemEvent.h"

#include "Global.h"

AddScheduleItemEvent::AddScheduleItemEvent(const ScheduleModel& model)
    : model(model)
{
}

AddScheduleItemEvent::AddScheduleItemEvent()
    : model(ScheduleModel(0,""))
{
    qDebug("Called AddScheduleItemEvent() constructor (Default Value)");
}

const ScheduleModel& AddScheduleItemEvent::getScheduleModel() const
{
    return this->model;
}

