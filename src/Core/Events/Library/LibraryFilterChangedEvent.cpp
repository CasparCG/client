#include "LibraryFilterChangedEvent.h"

#include "Global.h"

LibraryFilterChangedEvent::LibraryFilterChangedEvent(const QString& filter)
    : filter(filter)
{
}

const QString& LibraryFilterChangedEvent::getFilter() const
{
    return this->filter;
}
