#pragma once

#include "Shared.h"

#include <osc/OscReceivedElements.h>
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

#include <QtCore/QObject>

class OSC_EXPORT OscDevice : public QObject
{
    Q_OBJECT

    public:
        explicit OscDevice(const QString& address, int port = 6250, QObject* parent = 0);
        ~OscDevice();

        void connectDevice();
        void disconnectDevice();

        const int getPort() const;
        const QString getAddress() const;

    private:
            int port;
            QString address;
};
