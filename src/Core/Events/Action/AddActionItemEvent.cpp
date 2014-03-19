#include "AddActionItemEvent.h"

#include "Global.h"

AddActionItemEvent::AddActionItemEvent(const QString& action)
    : action(action)
{
}

const QString& AddActionItemEvent::getAction() const
{
    return this->action;
}
