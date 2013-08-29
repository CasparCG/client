#include "ChannelChangedEvent.h"

#include "Global.h"

ChannelChangedEvent::ChannelChangedEvent(int channel)
    : QEvent(static_cast<QEvent::Type>(Event::EventType::ChannelChanged)), channel(channel)
{
}

int ChannelChangedEvent::getChannel() const
{
    return this->channel;
}
