#include "RefreshLibraryEvent.h"

#include "Global.h"

RefreshLibraryEvent::RefreshLibraryEvent(int delay)
    : QEvent(static_cast<QEvent::Type>(Enum::EventType::RefreshLibrary)), delay(delay)
{
}

int RefreshLibraryEvent::getDelay() const
{
    return this->delay;
}
