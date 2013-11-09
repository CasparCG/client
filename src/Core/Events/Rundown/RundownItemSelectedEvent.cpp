#include "RundownItemSelectedEvent.h"

#include "Global.h"

RundownItemSelectedEvent::RundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model, QWidget* source, QWidget* parent)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::RundownItemSelected)), command(command), model(model), source(source), parent(parent)
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
