#include "RouteVideolayerCommand.h"

#include "Global.h"

RouteVideolayerCommand::RouteVideolayerCommand(QObject* parent)
    : AbstractCommand(parent),
      fromChannel(Route::DEFAULT_FROM_CHANNEL), fromVideolayer(Route::DEFAULT_FROM_VIDEOLAYER)
{
}

int RouteVideolayerCommand::getFromChannel() const
{
    return this->fromChannel;
}

int RouteVideolayerCommand::getFromVideolayer() const
{
    return this->fromVideolayer;
}

void RouteVideolayerCommand::setFromChannel(int fromChannel)
{
    this->fromChannel = fromChannel;
    emit fromChannelChanged(this->fromChannel);
}

void RouteVideolayerCommand::setFromVideolayer(int fromVideolayer)
{
    this->fromVideolayer = fromVideolayer;
    emit fromVideolayerChanged(this->fromVideolayer);
}

void RouteVideolayerCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setFromChannel(pt.get(L"fromchannel", Route::DEFAULT_FROM_CHANNEL));
    setFromVideolayer(pt.get(L"fromvideolayer", Route::DEFAULT_FROM_VIDEOLAYER));
}

void RouteVideolayerCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("fromchannel", QString::number(getFromChannel()));
    writer->writeTextElement("fromvideolayer", QString::number(getFromVideolayer()));
}
