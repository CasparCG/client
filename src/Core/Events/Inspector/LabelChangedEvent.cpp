#include "LabelChangedEvent.h"

#include "Global.h"

LabelChangedEvent::LabelChangedEvent(const QString& label)
    : label(label)
{
}

const QString& LabelChangedEvent::getLabel() const
{
    return this->label;
}
