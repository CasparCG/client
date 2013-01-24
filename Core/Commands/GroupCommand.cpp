#include "GroupCommand.h"

#include "Global.h"

GroupCommand::GroupCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI)
{
}

int GroupCommand::getDelay() const
{
    return this->delay;
}

int GroupCommand::getChannel() const
{
    return this->channel;
}

int GroupCommand::getVideolayer() const
{
    return this->videolayer;
}

void GroupCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void GroupCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void GroupCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

bool GroupCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void GroupCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void GroupCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
}

void GroupCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", "");
    writer->writeTextElement("videolayer", "");
    writer->writeTextElement("delay", "");
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
}
