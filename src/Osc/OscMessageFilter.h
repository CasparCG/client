#pragma once

#include <QPair>
#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include <boost/function.hpp>

#include "OscReceiver.h"
#include "Predicate.h"

/**
 * Utility class for selective subscription of the
 * OscReceiver::messageReceived() signal based on the OscInMessage::getPath()
 * values. A predicate with the signature "bool (QString)" determines what
 * messages to propagate. An additional predicate filters messages based on
 * their origin.
 *
 * If both the path predicate and the address/origin predicate returns true for
 * a message the signal interestingMessageArrived() will be emitted.
 *
 * See Predicate.h for some predifened predicates that can be used.
 */
class OSC_EXPORT OscMessageFilter : public QObject
{
    Q_OBJECT

    public:
        /**
         * Constructor which automatically connects the
         * interestingMessageArrived() signal to the receiver slot.
         *
         * By default messages no messages are accepted, regardless of path.
         * Use fromXX() methods to open up traffic.
         *
         * @param oscReceiver   the OSC receiver providing the unfiltered
         *                      messages.
         * @param receiver      the receiver object.
         * @param method        the slot on the receiver object to attach.
         * @param pathPredicate the predicate with a "bool (QString)"
         *                      compatible signarure. For example
         *                      Equals(QString("/foo/bar"))
         */
        template<typename PathPred>
        OscMessageFilter(
                const OscReceiver* oscReceiver,
                const QObject* receiver,
                const char* method,
                const PathPred pathPredicate)
            : oscReceiver(oscReceiver)
            , pathPredicate(pathPredicate)
            , addressPredicate(None<OscAddress>())
        {
            QObject::connect(this, SIGNAL(interestingMessageArrived(const OscInMessage&)), receiver, method);
            QObject::connect(
                    oscReceiver, SIGNAL(messageReceived(const OscInMessage&)),
                    this, SLOT(onMessage(OscInMessage)));
        }

        ~OscMessageFilter();

        /**
         * Short for from(IpOfOscAddress(Equals(address)))
         *
         * @param address The IP address.
         */
        void fromSpecificIp(const QString& address);

        /**
         * Short for from(Any<OscAddress>())
         */
        void fromAnyIp();

        /**
         * Filter OscInMessage::getAddress() based on a predicate compatible to
         * the signature "bool (OscAddress)".
         *
         * @param addressPredicate the predicate.
         */
        template<typename AddressPred>
        void from(const AddressPred& addressPredicate)
        {
            this->addressPredicate = addressPredicate;
        }

    private:
        Q_SIGNAL void interestingMessageArrived(const OscInMessage& message);
        Q_SLOT void onMessage(const OscInMessage& message);

        const OscReceiver* oscReceiver;
        boost::function<bool (const QString& path)> pathPredicate;
        boost::function<bool (const OscAddress& address)> addressPredicate;
};
