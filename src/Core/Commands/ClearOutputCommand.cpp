#include "ClearOutputCommand.h"

#include <QtCore/QXmlStreamWriter>

ClearOutputCommand::ClearOutputCommand(QObject* parent)
    : AbstractCommand(parent)
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

    setClearChannel(pt.get(L"clearchannel", ClearOutput::DEFAULT_CLEAR_CHANNEL));
    setTriggerOnNext(pt.get(L"triggeronnext", ClearOutput::DEFAULT_TRIGGER_ON_NEXT));
}

void ClearOutputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("clearchannel", (getClearChannel() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
