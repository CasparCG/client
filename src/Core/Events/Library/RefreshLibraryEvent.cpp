#include "RefreshLibraryEvent.h"

#include "Global.h"

RefreshLibraryEvent::RefreshLibraryEvent(int delay)
    : delay(delay)
{
}

int RefreshLibraryEvent::getDelay() const
{
    return this->delay;
}
