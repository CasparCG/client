#include "RundownItemSelectedEvent.h"

#include "Global.h"

RundownItemSelectedEvent::RundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model, QWidget* source, QWidget* parent)
    : command(command), model(model), source(source), parent(parent)
{
}

AbstractCommand* RundownItemSelectedEvent::getCommand() const
{
    return this->command;
}

LibraryModel* RundownItemSelectedEvent::getLibraryModel() const
{
    return this->model;
}

QWidget* RundownItemSelectedEvent::getSource() const
{
    return this->source;
}

QWidget* RundownItemSelectedEvent::getParent() const
{
    return this->parent;
}
