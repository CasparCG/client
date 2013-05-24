#include "OscMessageFilter.h"

#include <OscInMessage.h>

OscMessageFilter::~OscMessageFilter()
{
    QObject::disconnect(
            oscReceiver, SIGNAL(messageReceived(const OscInMessage&)),
            this, SLOT(onMessage(OscInMessage)));
}

void OscMessageFilter::fromSpecificIp(const QString& address)
{
    from(IpOfOscAddress(Equals(address)));
}

void OscMessageFilter::fromAnyIp()
{
    from(Any<OscAddress>());
}

void OscMessageFilter::onMessage(const OscInMessage& message)
{
    if (addressPredicate(message.getOrigin()) && pathPredicate(message.getPath()))
        emit interestingMessageArrived(message);
}
