#include "PreviewInputCommand.h"

#include "Global.h"

PreviewInputCommand::PreviewInputCommand(QObject* parent)
    : AbstractCommand(parent),
      input(TriCaster::DEFAULT_PREVIEW_INPUT)
{
}

void PreviewInputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);
}

void PreviewInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);
}

const QString& PreviewInputCommand::getInput() const
{
    return this->input;
}

void PreviewInputCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}
