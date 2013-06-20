#include "AddRudnownItemEvent.h"

#include "Global.h"

AddRudnownItemEvent::AddRudnownItemEvent(const LibraryModel& model)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AddRudnownItem)), model(model)
{
}

LibraryModel AddRudnownItemEvent::getLibraryModel()
{
    return this->model;
}
