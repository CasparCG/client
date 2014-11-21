#include "RouteChannelCommand.h"

#include "Global.h"

RouteChannelCommand::RouteChannelCommand(QObject* parent)
    : AbstractCommand(parent),
      fromChannel(Route::DEFAULT_FROM_CHANNEL)
{
}

int RouteChannelCommand::getFromChannel() const
{
    return this->fromChannel;
}

void RouteChannelCommand::setFromChannel(int fromChannel)
{
    this->fromChannel = fromChannel;
    emit fromChannelChanged(this->fromChannel);
}

void RouteChannelCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFromChannel(pt.get(L"fromchannel", Route::DEFAULT_FROM_CHANNEL));
}

void RouteChannelCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("fromchannel", QString::number(getFromChannel()));
}
