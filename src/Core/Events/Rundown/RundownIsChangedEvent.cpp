#include "RundownIsChangedEvent.h"

#include "Global.h"

RundownIsChangedEvent::RundownIsChangedEvent(bool changed)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::RundownIsChanged)), changed(changed)
{
}

bool RundownIsChangedEvent::getChanged() const
{
    return this->changed;
}
