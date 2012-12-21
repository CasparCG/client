#include "OpenRundownEvent.h"

#include "Global.h"

OpenRundownEvent::OpenRundownEvent(const QString& path)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::OpenRundown)),
      path(path)
{
}

const QString& OpenRundownEvent::getPath() const
{
    return this->path;
}
