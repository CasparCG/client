#pragma once

#include "Shared.h"
#include "OscThread.h"

#include <osc/OscReceivedElements.h>
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

#include <QtCore/QObject>

#include <QtNetwork/QHostAddress>

class OSC_EXPORT OscListener : public QObject, public osc::OscPacketListener
{
    Q_OBJECT

    public:
        explicit OscListener(const QString& address, int port, QObject* parent = 0);
        ~OscListener();

        void start();

        Q_SIGNAL void oscMessageReceived(const QString&, const QString&, const QList<QVariant>&);

    protected:
        virtual void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint);

    private:
        OscThread* thread;
        UdpSocket* socket;
        SocketReceiveMultiplexer* multiplexer;
};
