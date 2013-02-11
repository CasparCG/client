#include "ClearOutputCommand.h"

#include "Global.h"

ClearOutputCommand::ClearOutputCommand(QObject* parent)
    : AbstractCommand(parent),
      clearChannel(ClearOutput::DEFAULT_CLEAR_CHANNEL)
{
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
