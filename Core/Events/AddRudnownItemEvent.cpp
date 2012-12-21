#include "AddRudnownItemEvent.h"

#include "Global.h"

AddRudnownItemEvent::AddRudnownItemEvent(const LibraryModel& model)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::AddRudnownItem)), model(model)
{
}

LibraryModel AddRudnownItemEvent::getLibraryModel()
{
    return this->model;
}
