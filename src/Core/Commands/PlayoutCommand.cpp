#include "PlayoutCommand.h"

#include <QtCore/QXmlStreamWriter>

PlayoutCommand::PlayoutCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

const QString& PlayoutCommand::getPlayoutCommand() const
{
    return this->command;
}

void PlayoutCommand::setPlayoutCommand(const QString& command)
{
    this->command = command;
    emit playoutCommandChanged(this->command);
}

void PlayoutCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setPlayoutCommand(QString::fromStdWString(pt.get(L"playoutcommand", Output::DEFAULT_PLAYOUT_COMMAND.toStdWString())));
}

void PlayoutCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("playoutcommand", this->getPlayoutCommand());
}
