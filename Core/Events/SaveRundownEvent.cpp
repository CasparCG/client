#include "SaveRundownEvent.h"

#include "Global.h"

SaveRundownEvent::SaveRundownEvent(bool saveAs)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::SaveRundown)),
      saveAs(saveAs)
{
}

bool SaveRundownEvent::getSaveAs() const
{
    return this->saveAs;
}
