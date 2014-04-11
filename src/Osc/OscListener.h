#pragma once

#include "Shared.h"
#include "OscThread.h"

#include <osc/OscReceivedElements.h>
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QMutex>

class OSC_EXPORT OscListener : public QObject, public osc::OscPacketListener
{
    Q_OBJECT

    public:
        explicit OscListener(const QString& address, int port, QObject* parent = 0);
        ~OscListener();

        void start();

        Q_SIGNAL void messageReceived(const QString&, const QList<QVariant>&);

    protected:
        virtual void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint);

    private:
        QMutex eventsMutex;
        QMap<QString, QList<QVariant> > events;
        OscThread* thread;
        UdpSocket* socket;
        SocketReceiveMultiplexer* multiplexer;

        Q_SLOT void sendEventBatch();
};
