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
