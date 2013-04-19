#include "LabelChangedEvent.h"

#include "Global.h"

LabelChangedEvent::LabelChangedEvent(const QString& label)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::LabelChanged)), label(label)
{
}

const QString& LabelChangedEvent::getLabel() const
{
    return this->label;
}
