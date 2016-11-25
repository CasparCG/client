#include "PrintCommand.h"

#include <QtCore/QXmlStreamWriter>

PrintCommand::PrintCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& PrintCommand::getOutput() const
{
    return this->output;
}

void PrintCommand::setOutput(const QString& output)
{
    this->output = output;
    emit outputChanged(this->output);
}

void PrintCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setOutput(QString::fromStdWString(pt.get(L"output", Print::DEFAULT_OUTPUT.toStdWString())));
}

void PrintCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("output", this->getOutput());
}
