#include "ClearOutputCommand.h"

#include "Global.h"

ClearOutputCommand::ClearOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), clearChannel(ClearOutput::DEFAULT_CLEAR_CHANNEL)
{
}

int ClearOutputCommand::getDelay() const
{
    return this->delay;
}

int ClearOutputCommand::getChannel() const
{
    return this->channel;
}

int ClearOutputCommand::getVideolayer() const
{
    return this->videolayer;
}

void ClearOutputCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void ClearOutputCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void ClearOutputCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

bool ClearOutputCommand::getClearChannel() const
{
    return this->clearChannel;
}

void ClearOutputCommand::setClearChannel(bool clearChannel)
{
    this->clearChannel = clearChannel;
    emit clearChannelChanged(this->clearChannel);
}

bool ClearOutputCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void ClearOutputCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void ClearOutputCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"clearchannel") > 0) setClearChannel(pt.get<bool>(L"clearchannel"));
}

void ClearOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("clearchannel", (getClearChannel() == true) ? "true" : "false");
}
