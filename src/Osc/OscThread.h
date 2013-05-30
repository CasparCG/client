#pragma once

#include "Shared.h"

#include <ip/UdpSocket.h>

#include <QtCore/QObject>
#include <QtCore/QThread>

class OSC_EXPORT OscThread : public QThread
{
    Q_OBJECT

    public:
        explicit OscThread(SocketReceiveMultiplexer* multiplexer, QObject* parent = 0);

        void stop();

    protected:
        void run();

    private:
        SocketReceiveMultiplexer* multiplexer;
};
