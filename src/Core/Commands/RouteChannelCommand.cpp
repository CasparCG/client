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

int RouteChannelCommand::getOutputDelay() const
{
    return this->outputDelay;
}

void RouteChannelCommand::setFromChannel(int fromChannel)
{
    this->fromChannel = fromChannel;
    emit fromChannelChanged(this->fromChannel);
}

void RouteChannelCommand::setOutputDelay(int delay)
{
    this->outputDelay = delay;
    emit outputDelayChanged(this->outputDelay);
}

void RouteChannelCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFromChannel(pt.get(L"fromchannel", Route::DEFAULT_FROM_CHANNEL));
    setOutputDelay(pt.get(L"outputdelay", Route::DEFAULT_OUTPUT_DELAY));
}

void RouteChannelCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("fromchannel", QString::number(getFromChannel()));
    writer.writeTextElement("outputdelay", QString::number(getOutputDelay()));
}
