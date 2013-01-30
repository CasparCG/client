#include "KeyerCommand.h"

#include "Global.h"

KeyerCommand::KeyerCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), defer(Mixer::DEFAULT_DEFER)
{
}

int KeyerCommand::getDelay() const
{
    return this->delay;
}

int KeyerCommand::getChannel() const
{
    return this->channel;
}

int KeyerCommand::getVideolayer() const
{
    return this->videolayer;
}

void KeyerCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void KeyerCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void KeyerCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

bool KeyerCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void KeyerCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

bool KeyerCommand::getDefer() const
{
    return this->defer;
}

void KeyerCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void KeyerCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"defer") > 0) setDefer(pt.get<int>(L"defer"));
}

void KeyerCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("defer", QString::number(this->getDefer()));
}
