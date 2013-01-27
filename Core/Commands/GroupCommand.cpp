#include "GroupCommand.h"

#include "Global.h"

GroupCommand::GroupCommand(QObject* parent)
    : QObject(parent),
      allowGpi(Output::DEFAULT_ALLOW_GPI)
{
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
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
}
