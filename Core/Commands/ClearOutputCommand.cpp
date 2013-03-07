#include "ClearOutputCommand.h"

#include "Global.h"

ClearOutputCommand::ClearOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      clearChannel(ClearOutput::DEFAULT_CLEAR_CHANNEL), triggerOnNext(Media::DEFAULT_TRIGGER_ON_NEXT)
{
}

bool ClearOutputCommand::getClearChannel() const
{
    return this->clearChannel;
}

bool ClearOutputCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void ClearOutputCommand::setClearChannel(bool clearChannel)
{
    this->clearChannel = clearChannel;
    emit clearChannelChanged(this->clearChannel);
}

void ClearOutputCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void ClearOutputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"clearchannel") > 0) setClearChannel(pt.get<bool>(L"clearchannel"));
}

void ClearOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("clearchannel", (getClearChannel() == true) ? "true" : "false");
}
