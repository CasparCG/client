#include "CurrentItemChangedEvent.h"

#include "Global.h"

CurrentItemChangedEvent::CurrentItemChangedEvent(QTreeWidgetItem* current, QTreeWidgetItem* previous)
    : current(current), previous(previous)
{
}

QTreeWidgetItem* CurrentItemChangedEvent::getCurrentItem() const
{
    return this->current;
}

QTreeWidgetItem* CurrentItemChangedEvent::getPreviousItem() const
{
    return this->previous;
}
