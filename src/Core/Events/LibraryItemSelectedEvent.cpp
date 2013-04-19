#include "LibraryItemSelectedEvent.h"

#include "Global.h"

LibraryItemSelectedEvent::LibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected)), command(command), model(model)
{
}

AbstractCommand* LibraryItemSelectedEvent::getCommand() const
{
    return this->command;
}

LibraryModel* LibraryItemSelectedEvent::getLibraryModel() const
{
    return this->model;
}
