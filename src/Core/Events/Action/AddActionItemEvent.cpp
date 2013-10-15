#include "AddActionItemEvent.h"

#include "Global.h"

AddActionItemEvent::AddActionItemEvent(const QString& action)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AddActionItem)), action(action)
{
}

const QString& AddActionItemEvent::getAction() const
{
    return this->action;
}
