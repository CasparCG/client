#include "OscListener.h"

#include <QtCore/QDebug>

OscListener::OscListener(QObject* parent)
    : QObject(parent)
{
}

OscListener::~OscListener()
{
}

void OscListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint)
{
    qDebug() << message.AddressPattern();
}
