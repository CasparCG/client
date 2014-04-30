#include "ReloadRundownEvent.h"

#include "Global.h"

ReloadRundownEvent::ReloadRundownEvent(const QString& path)
    : path(path)
{
}

const QString& ReloadRundownEvent::getPath() const
{
    return this->path;
}
