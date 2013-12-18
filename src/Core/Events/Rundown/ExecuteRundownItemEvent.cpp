#include "ExecuteRundownItemEvent.h"

ExecuteRundownItemEvent::ExecuteRundownItemEvent(Playout::PlayoutType::Type type, QTreeWidgetItem* item)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::ExecuteRundownItem)), type(type), item(item)
{
}

Playout::PlayoutType::Type ExecuteRundownItemEvent::getType() const
{
    return this->type;
}

QTreeWidgetItem* ExecuteRundownItemEvent::getItem() const
{
    return this->item;
}
