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

class OSC_EXPORT OscControlListener : public QObject, public osc::OscPacketListener
{
    Q_OBJECT

    public:
        explicit OscControlListener(QObject* parent = 0);
        ~OscControlListener();

        void start(int port);

        Q_SIGNAL void messageReceived(const QString&, const QList<QVariant>&);

    protected:
        virtual void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& endpoint);

    private:
        int port;
        QMutex eventsMutex;
        QMap<QString, QList<QVariant>> events;
        OscThread* thread = nullptr;
        UdpSocket* socket = nullptr;
        SocketReceiveMultiplexer* multiplexer = nullptr;

        Q_SLOT void sendEventBatch();
};
