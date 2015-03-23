#include "VideolayerChangedEvent.h"

#include "Global.h"

VideolayerChangedEvent::VideolayerChangedEvent(int videolayer)
    : videolayer(videolayer)
{
}

int VideolayerChangedEvent::getVideolayer() const
{
    return this->videolayer;
}
