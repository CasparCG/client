#include "TargetChangedEvent.h"

#include "Global.h"

TargetChangedEvent::TargetChangedEvent(const QString& target)
    : target(target)
{
}

const QString& TargetChangedEvent::getTarget() const
{
    return this->target;
}
