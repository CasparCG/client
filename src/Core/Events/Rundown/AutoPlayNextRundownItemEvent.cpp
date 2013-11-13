#include "AutoPlayNextRundownItemEvent.h"

#include "Global.h"

AutoPlayNextRundownItemEvent::AutoPlayNextRundownItemEvent(QWidget* widget)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AutoPlayNextRundownItem)), widget(widget)
{
}

QWidget* AutoPlayNextRundownItemEvent::getSource() const
{
    return this->widget;
}
