#pragma once

#include "Shared.h"

#include <QtCore/QObject>
#include <QSharedPointer>

#include "OscInMessage.h"

/**
 * An OSC Receiver that binds to an UDP port on the machine.
 */
class OSC_EXPORT OscReceiver : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         *
         * @param address must be the IP address of a local interface on this machine.
         */
        explicit OscReceiver(const OscAddress& address, QObject* parent = 0);
        ~OscReceiver();

        void start();
        void stop();

        const OscAddress& getAddress() const;

        Q_SIGNAL void messageReceived(const OscInMessage& message);

    private:
        struct Implementation;
        QSharedPointer<Implementation> impl;
};
