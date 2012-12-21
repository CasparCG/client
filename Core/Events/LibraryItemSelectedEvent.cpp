#include "LibraryItemSelectedEvent.h"

#include "Global.h"

LibraryItemSelectedEvent::LibraryItemSelectedEvent(ICommand* command, LibraryModel* model)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected)), command(command), model(model)
{
}

ICommand* LibraryItemSelectedEvent::getCommand() const
{
    return this->command;
}

LibraryModel* LibraryItemSelectedEvent::getLibraryModel() const
{
    return this->model;
}
