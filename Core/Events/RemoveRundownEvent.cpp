#include "RemoveRundownEvent.h"

#include "Global.h"

RemoveRundownEvent::RemoveRundownEvent(int index)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RemoveRundown)), index(index)
{
}

int RemoveRundownEvent::getIndex() const
{
    return this->index;
}
