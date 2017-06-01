#include "RouteChannelCommand.h"

#include <QtCore/QXmlStreamWriter>

RouteChannelCommand::RouteChannelCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

int RouteChannelCommand::getFromChannel() const
{
    return this->fromChannel;
}

int RouteChannelCommand::getRouteDelay() const
{
    return this->routeDelay;
}

void RouteChannelCommand::setFromChannel(int fromChannel)
{
    this->fromChannel = fromChannel;
    emit fromChannelChanged(this->fromChannel);
}

void RouteChannelCommand::setRouteDelay(int delay)
{
    this->routeDelay = delay;
    emit routeDelayChanged(this->routeDelay);
}

void RouteChannelCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFromChannel(pt.get(L"fromchannel", Route::DEFAULT_FROM_CHANNEL));
    setRouteDelay(pt.get(L"routedelay", Route::DEFAULT_ROUTE_DELAY));
}

void RouteChannelCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("fromchannel", QString::number(getFromChannel()));
    writer.writeTextElement("routedelay", QString::number(getRouteDelay()));
}
