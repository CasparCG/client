#pragma once

#include "Shared.h"

#include <functional>

#include <QString>
#include <QVariant>
#include <QVector>

class OSC_EXPORT OscAddress
{
    public:
        explicit OscAddress(const QString& ipAddress, int port = 6250);

        const QString& getIpAddress() const;
        int getPort() const;
    private:
        const QString ipAddress;
        const int port;
};

class OSC_EXPORT OscInMessage
{
    public:
        // Default constructibility required by QT for some reason when passed as signal/slot argument
        OscInMessage();
        OscInMessage(const OscAddress& origin, const QString& path, const QVector<QVariant>& arguments);

        const OscAddress& getOrigin() const;
        const QString& getPath() const;
        const QVector<QVariant>& getArguments() const;
    private:
        const OscAddress origin;
        const QString path;
        const QVector<QVariant> arguments;
};

template<typename P>
class IpOfOscAddressPredicate : public std::unary_function<const OscAddress&, bool>
{
    public:
        IpOfOscAddressPredicate(const P& predicate)
            : predicate(predicate)
        {
        }

        bool operator()(const OscAddress& elem) const
        {
            return predicate(elem.getIpAddress());
        }
    private:
        P predicate;
};

template<typename P>
IpOfOscAddressPredicate<P> IpOfOscAddress(const P& predicate)
{
    return IpOfOscAddressPredicate<P>(predicate);
}

Q_DECLARE_METATYPE(OscInMessage)
