#include "SaveRundownEvent.h"

#include "Global.h"

SaveRundownEvent::SaveRundownEvent(const QString& path)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::SaveRundown)),
      path(path)
{
}

const QString& SaveRundownEvent::getPath() const
{
    return this->path;
}
