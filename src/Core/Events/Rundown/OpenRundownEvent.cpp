#include "OpenRundownEvent.h"

#include "Global.h"

OpenRundownEvent::OpenRundownEvent(const QString& path)
    : path(path)
{
}

const QString& OpenRundownEvent::getPath() const
{
    return this->path;
}
