#include "SaveRundownEvent.h"

#include "Global.h"

SaveRundownEvent::SaveRundownEvent(bool saveAs)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::SaveRundown)),
      saveAs(saveAs)
{
}

bool SaveRundownEvent::getSaveAs() const
{
    return this->saveAs;
}
