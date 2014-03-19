#include "LibraryItemSelectedEvent.h"

#include "Global.h"

LibraryItemSelectedEvent::LibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model)
    : command(command), model(model)
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
