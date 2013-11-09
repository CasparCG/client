#include "AutoPlayRundownItemEvent.h"

#include "Global.h"

AutoPlayRundownItemEvent::AutoPlayRundownItemEvent(QWidget* widget)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AutoPlayRundownItem)), widget(widget)
{
}

QWidget* AutoPlayRundownItemEvent::getSource() const
{
    return this->widget;
}
