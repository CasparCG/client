#include "SaveRundownEvent.h"

#include "Global.h"

SaveRundownEvent::SaveRundownEvent(bool saveAs)
    : saveAs(saveAs)
{
}

bool SaveRundownEvent::getSaveAs() const
{
    return this->saveAs;
}
