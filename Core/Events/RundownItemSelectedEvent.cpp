#include "RundownItemSelectedEvent.h"

#include "Global.h"

RundownItemSelectedEvent::RundownItemSelectedEvent(ICommand* command, LibraryModel* model)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected)), command(command), model(model)
{
}

ICommand* RundownItemSelectedEvent::getCommand() const
{
    return this->command;
}

LibraryModel* RundownItemSelectedEvent::getLibraryModel() const
{
    return this->model;
}
