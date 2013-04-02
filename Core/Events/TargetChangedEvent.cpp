#include "TargetChangedEvent.h"

#include "Global.h"

TargetChangedEvent::TargetChangedEvent(const QString& name)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::TargetChanged)), name(name)
{
}

const QString& TargetChangedEvent::getName() const
{
    return this->name;
}
