#include "CompactViewEvent.h"

#include "Global.h"

CompactViewEvent::CompactViewEvent(bool enabled)
    : enabled(enabled)
{
}

bool CompactViewEvent::getEnabled() const
{
    return this->enabled;
}
