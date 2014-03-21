#include "RemoveItemFromAutoPlayQueueEvent.h"

#include "Global.h"

RemoveItemFromAutoPlayQueueEvent::RemoveItemFromAutoPlayQueueEvent(QTreeWidgetItem* item)
    : item(item)
{
}

QTreeWidgetItem* RemoveItemFromAutoPlayQueueEvent::getItem() const
{
    return this->item;
}
