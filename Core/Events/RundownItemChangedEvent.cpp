#include "RundownItemChangedEvent.h"

#include "Global.h"

RundownItemChangedEvent::RundownItemChangedEvent(const QString& name, const QString& deviceName)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged)), name(name), deviceName(deviceName)
{
}

const QString& RundownItemChangedEvent::getName() const
{
    return this->name;
}

const QString& RundownItemChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
