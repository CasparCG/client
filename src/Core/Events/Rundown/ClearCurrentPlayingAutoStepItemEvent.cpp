#include "ClearCurrentPlayingAutoStepItemEvent.h"

#include "Global.h"

ClearCurrentPlayingAutoStepItemEvent::ClearCurrentPlayingAutoStepItemEvent(QTreeWidgetItem* item)
    : item(item)
{
}

QTreeWidgetItem* ClearCurrentPlayingAutoStepItemEvent::getItem() const
{
    return this->item;
}
