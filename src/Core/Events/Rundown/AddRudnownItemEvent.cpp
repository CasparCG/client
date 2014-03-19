#include "AddRudnownItemEvent.h"

#include "Global.h"

AddRudnownItemEvent::AddRudnownItemEvent(const LibraryModel& model)
    : model(model)
{
}

LibraryModel AddRudnownItemEvent::getLibraryModel() const
{
    return this->model;
}
