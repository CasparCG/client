#pragma once

#include "Shared.h"

#include "osc/OscOutboundPacketStream.h"
#include <ip/UdpSocket.h>

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QMutex>

class OSC_EXPORT OscSender : public QObject
{
    Q_OBJECT

    public:
        explicit OscSender(QObject* parent = 0);
        ~OscSender();

        void send(const QString& address, int port, const QString& path, const QVariant& message, bool useBundle);

    private:
        bool useBundle;
};
