#include "OscReceiver.h"

#include <QString>
#include <QThread>

#include <osc/OscReceivedElements.h>
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

struct ArgumentConverter
{
    virtual ~ArgumentConverter()
    {
    }

    virtual bool canHandle(const osc::ReceivedMessageArgument& arg) const = 0;
    virtual QVariant convert(const osc::ReceivedMessageArgument& arg) const = 0;
};

class CompositeArgumentConverter : public ArgumentConverter
{
private:
    QVector<QSharedPointer<ArgumentConverter> > converters;
public:
    CompositeArgumentConverter& add(const QSharedPointer<ArgumentConverter> converter)
    {
        converters.push_back(converter);

        return *this;
    }

    template<typename C>
    CompositeArgumentConverter& add()
    {
        add(QSharedPointer<ArgumentConverter>(new C));

        return *this;
    }

    bool canHandle(const osc::ReceivedMessageArgument& arg) const
    {
        foreach (const QSharedPointer<ArgumentConverter>& converter, converters)
        {
            if (converter->canHandle(arg))
                return true;
        }

        return false;
    }

    QVariant convert(const osc::ReceivedMessageArgument& arg) const
    {
        foreach (const QSharedPointer<ArgumentConverter>& converter, converters)
        {
            if (converter->canHandle(arg))
                return converter->convert(arg);
        }

        // Failed
        return QVariant();
    }
};

struct BoolArgumentConverter : public ArgumentConverter
{
    bool canHandle(const osc::ReceivedMessageArgument& arg) const   { return arg.IsBool(); }
    QVariant convert(const osc::ReceivedMessageArgument& arg) const { return arg.AsBool(); }
};

struct Int64ArgumentConverter : public ArgumentConverter
{
    bool canHandle(const osc::ReceivedMessageArgument& arg) const   { return arg.IsInt64(); }
    QVariant convert(const osc::ReceivedMessageArgument& arg) const { return arg.AsInt64(); }
};

struct FloatArgumentConverter : public ArgumentConverter
{
    bool canHandle(const osc::ReceivedMessageArgument& arg) const   { return arg.IsFloat(); }
    QVariant convert(const osc::ReceivedMessageArgument& arg) const { return arg.AsFloat(); }
};

struct DoubleArgumentConverter : public ArgumentConverter
{
    bool canHandle(const osc::ReceivedMessageArgument& arg) const   { return arg.IsDouble(); }
    QVariant convert(const osc::ReceivedMessageArgument& arg) const { return arg.AsDouble(); }
};

struct StringArgumentConverter : public ArgumentConverter
{
    bool canHandle(const osc::ReceivedMessageArgument& arg) const   { return arg.IsString(); }
    QVariant convert(const osc::ReceivedMessageArgument& arg) const { return QString(arg.AsString()); }
};

// TODO support more datatypes if the need arises.

class OSC_EXPORT OscEventLoopThread : public QThread
{
    Q_OBJECT

    SocketReceiveMultiplexer& multiplexer;
public:
    OscEventLoopThread(SocketReceiveMultiplexer& multiplexer)
        : multiplexer(multiplexer)
    {
    }

    void run()
    {
        multiplexer.Run();
    }
};

struct OscReceiver::Implementation : public osc::OscPacketListener
{
    OscAddress address;
    SocketReceiveMultiplexer multiplexer;
    UdpSocket socket;
    OscEventLoopThread thread;

    Implementation(const OscAddress& address, OscReceiver& self)
        : address(address)
        , thread(multiplexer)
        , self(self)
    {
        socket.SetAllowReuse(true);
        socket.Bind(IpEndpointName(
                address.getIpAddress().toStdString().c_str(),
                address.getPort()));
        multiplexer.AttachSocketListener(&socket, this);
        converter
            .add<BoolArgumentConverter>()
            .add<Int64ArgumentConverter>()
            .add<FloatArgumentConverter>()
            .add<DoubleArgumentConverter>()
            .add<StringArgumentConverter>()
            ;
    }

protected:
    virtual void ProcessMessage(
            const osc::ReceivedMessage& msg, const IpEndpointName& endpoint)
    {
        char addressBuffer[256];
        endpoint.AddressAsString(addressBuffer);
        OscAddress origin(QString(addressBuffer), endpoint.port);
        QString path(msg.AddressPattern());
        QVector<QVariant> arguments;
        arguments.reserve(msg.ArgumentCount());

        typedef osc::ReceivedMessage::const_iterator const_iter;

        for (const_iter iter = msg.ArgumentsBegin(); iter != msg.ArgumentsEnd(); ++iter)
        {
            const osc::ReceivedMessageArgument& arg = *iter;

            arguments.push_back(converter.convert(arg));
        }

        OscInMessage message(origin, path, arguments);
        emit self.messageReceived(message);
    }
private:
    OscReceiver& self;
    CompositeArgumentConverter converter;
};

OscReceiver::OscReceiver(const OscAddress& address, QObject* parent)
    : QObject(parent), impl(new Implementation(address, *this))
{
    qRegisterMetaType<OscInMessage>("OscInMessage");
}

OscReceiver::~OscReceiver()
{
    stop();
    impl->thread.wait();
}

void OscReceiver::start()
{
    impl->thread.start();
}

void OscReceiver::stop()
{
    impl->multiplexer.Break();
}

const OscAddress& OscReceiver::getAddress() const
{
    return impl->address;
}

#include "OscReceiver.moc"
