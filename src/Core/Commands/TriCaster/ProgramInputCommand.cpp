#include "ProgramInputCommand.h"

#include "Global.h"

ProgramInputCommand::ProgramInputCommand(QObject* parent)
    : AbstractCommand(parent),
      input(TriCaster::DEFAULT_PROGRAM_INPUT)
{
}

void ProgramInputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);
}

void ProgramInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);
}

const QString& ProgramInputCommand::getInput() const
{
    return this->input;
}

void ProgramInputCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}
