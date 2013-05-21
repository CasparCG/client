#pragma once

#include "Shared.h"

#include <osc/OscReceivedElements.h>
#include <osc/OscPacketListener.h>

#include <QtCore/QObject>

class OSC_EXPORT OscListener : public QObject, public osc::OscPacketListener
{
    Q_OBJECT

    public:
        explicit OscListener(QObject* parent);
        ~OscListener();

    protected:
        virtual void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint);
};
