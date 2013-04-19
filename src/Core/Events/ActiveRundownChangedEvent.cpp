#include "ActiveRundownChangedEvent.h"

#include "Global.h"

ActiveRundownChangedEvent::ActiveRundownChangedEvent(const QString& path)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::ActiveRundownChanged)), path(path)
{
}

const QString& ActiveRundownChangedEvent::getPath() const
{
    return this->path;
}
