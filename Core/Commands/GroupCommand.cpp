#include "GroupCommand.h"

#include "Global.h"

GroupCommand::GroupCommand(QObject* parent)
    : AbstractCommand(parent),
      notes(Group::DEFAULT_NOTE), autoStep(Group::DEFAULT_AUTO_STEP)
{
}

bool GroupCommand::getAutoStep() const
{
    return this->autoStep;
}

const QString& GroupCommand::getNotes() const
{
    return this->notes;
}

void GroupCommand::setAutoStep(bool autoStep)
{
    this->autoStep = autoStep;
    emit autoStepChanged(this->autoStep);
}

void GroupCommand::setNotes(const QString& notes)
{
    this->notes = notes;
    emit notesChanged(this->notes);
}

void GroupCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"notes") > 0) setNotes(QString::fromStdWString(pt.get<std::wstring>(L"notes")));
    if (pt.count(L"autostep") > 0) setAutoStep(pt.get<bool>(L"autostep"));
}

void GroupCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("notes", this->getNotes());
    writer->writeTextElement("autostep", (getAutoStep() == true) ? "true" : "false");
}
