#include "AutoPlayChangedEvent.h"

#include "Global.h"

AutoPlayChangedEvent::AutoPlayChangedEvent(bool autoPlay)
    : autoPlay(autoPlay)
{
}

bool AutoPlayChangedEvent::getAutoPlay() const
{
    return this->autoPlay;
}
