#include "AutoPlayChangedEvent.h"

#include "Global.h"

AutoPlayChangedEvent::AutoPlayChangedEvent(bool autoPlay)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::AutoPlayChanged)), autoPlay(autoPlay)
{
}

bool AutoPlayChangedEvent::getAutoPlay() const
{
    return this->autoPlay;
}
