#include "ChannelChangedEvent.h"

#include "Global.h"

ChannelChangedEvent::ChannelChangedEvent(int channel)
    : channel(channel)
{
}

int ChannelChangedEvent::getChannel() const
{
    return this->channel;
}
