#include "AutoPlayRundownItemEvent.h"

#include "Global.h"

AutoPlayRundownItemEvent::AutoPlayRundownItemEvent(QWidget* widget)
    : widget(widget)
{
}

QWidget* AutoPlayRundownItemEvent::getSource() const
{
    return this->widget;
}
