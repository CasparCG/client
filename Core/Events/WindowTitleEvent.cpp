#include "WindowTitleEvent.h"

#include "Global.h"

WindowTitleEvent::WindowTitleEvent(const QString& title)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::WindowTitle)), title(title)
{
}

const QString& WindowTitleEvent::getTitle() const
{
    return this->title;
}
