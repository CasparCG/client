#include "AutoPlayNextRundownItemEvent.h"

#include "Global.h"

AutoPlayNextRundownItemEvent::AutoPlayNextRundownItemEvent(QWidget* widget)
    : widget(widget)
{
}

QWidget* AutoPlayNextRundownItemEvent::getSource() const
{
    return this->widget;
}
