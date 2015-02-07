#include "ClearCurrentPlayingItemEvent.h"

#include "Global.h"

ClearCurrentPlayingItemEvent::ClearCurrentPlayingItemEvent(QTreeWidgetItem* item)
    : item(item)
{
}

QTreeWidgetItem* ClearCurrentPlayingItemEvent::getItem() const
{
    return this->item;
}
