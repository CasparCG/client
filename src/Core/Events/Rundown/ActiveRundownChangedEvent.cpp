#include "ActiveRundownChangedEvent.h"

#include "Global.h"

ActiveRundownChangedEvent::ActiveRundownChangedEvent(const QString& path)
    : path(path)
{
}

const QString& ActiveRundownChangedEvent::getPath() const
{
    return this->path;
}
