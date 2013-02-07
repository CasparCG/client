#include "GroupCommand.h"

#include "Global.h"

GroupCommand::GroupCommand(QObject* parent)
    : AbstractCommand(parent),
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

const QString& GroupCommand::getNotes() const
{
    return this->notes;
}

void GroupCommand::setNotes(const QString& notes)
{
    this->notes = notes;
    emit notesChanged(this->notes);
}

void GroupCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"notes") > 0) setNotes(QString::fromStdWString(pt.get<std::wstring>(L"notes")));
}

void GroupCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("notes", this->getNotes());
}
