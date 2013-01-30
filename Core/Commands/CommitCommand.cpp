#include "CommitCommand.h"

#include "Global.h"

CommitCommand::CommitCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), delay(Output::DEFAULT_DELAY), allowGpi(Output::DEFAULT_ALLOW_GPI)
{
}

int CommitCommand::getDelay() const
{
    return this->delay;
}

bool CommitCommand::getAllowGpi() const
{
    return this->allowGpi;
}

int CommitCommand::getChannel() const
{
    return this->channel;
}

void CommitCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void CommitCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void CommitCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void CommitCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
}

void CommitCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
}
