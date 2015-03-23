#include "OpenRundownFromUrlEvent.h"

#include "Global.h"

OpenRundownFromUrlEvent::OpenRundownFromUrlEvent(const QString& path)
    : path(path)
{
}

const QString& OpenRundownFromUrlEvent::getPath() const
{
    return this->path;
}
