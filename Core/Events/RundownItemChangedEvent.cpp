#include "RundownItemChangedEvent.h"

#include "Global.h"

RundownItemChangedEvent::RundownItemChangedEvent(const QString& label, const QString& name, const QString& deviceName)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RundownItemChanged)), label(label), name(name), deviceName(deviceName)
{
}

const QString& RundownItemChangedEvent::getLabel() const
{
    return this->label;
}

const QString& RundownItemChangedEvent::getName() const
{
    return this->name;
}

const QString& RundownItemChangedEvent::getDeviceName() const
{
    return this->deviceName;
}
