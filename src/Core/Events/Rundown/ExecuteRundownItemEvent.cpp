#include "ExecuteRundownItemEvent.h"

ExecuteRundownItemEvent::ExecuteRundownItemEvent(Playout::PlayoutType::Type type, QTreeWidgetItem* item, const QString& address)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::ExecuteRundownItem)), type(type), item(item), address(address)
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

QString ExecuteRundownItemEvent::getAddress() const
{
    return this->address;
}
