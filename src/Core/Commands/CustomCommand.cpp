#include "CustomCommand.h"

#include "Global.h"

CustomCommand::CustomCommand(QObject* parent)
    : AbstractCommand(parent),
      stopCommand(""), playCommand(""), loadCommand(""), pauseCommand(""), nextCommand(""), updateCommand(""),
      invokeCommand(""), clearCommand(""), clearVideolayerCommand(""), clearChannelCommand("")
{
}

const QString& CustomCommand::getStopCommand() const
{
    return this->stopCommand;
}

const QString& CustomCommand::getPlayCommand() const
{
    return this->playCommand;
}

const QString& CustomCommand::getLoadCommand() const
{
    return this->loadCommand;
}

const QString& CustomCommand::getPauseCommand() const
{
    return this->pauseCommand;
}

const QString& CustomCommand::getNextCommand() const
{
    return this->pauseCommand;
}

const QString& CustomCommand::getUpdateCommand() const
{
    return this->updateCommand;
}

const QString& CustomCommand::getInvokeCommand() const
{
    return this->invokeCommand;
}

const QString& CustomCommand::getClearCommand() const
{
    return this->clearCommand;
}

const QString& CustomCommand::getClearVideolayerCommand() const
{
    return this->clearVideolayerCommand;
}

const QString& CustomCommand::getClearChannelCommand() const
{
    return this->clearChannelCommand;
}

void CustomCommand::setStopCommand(const QString& command)
{
    this->stopCommand = command;
    emit stopCommandChanged(this->stopCommand);
}

void CustomCommand::setPlayCommand(const QString& command)
{
    this->playCommand = command;
    emit playCommandChanged(this->playCommand);
}

void CustomCommand::setLoadCommand(const QString& command)
{
    this->loadCommand = command;
    emit loadCommandChanged(this->loadCommand);
}

void CustomCommand::setPauseCommand(const QString& command)
{
    this->pauseCommand = command;
    emit pauseCommandChanged(this->pauseCommand);
}

void CustomCommand::setNextCommand(const QString& command)
{
    this->nextCommand = command;
    emit nextCommandChanged(this->nextCommand);
}

void CustomCommand::setUpdateCommand(const QString& command)
{
    this->updateCommand = command;
    emit updateCommandChanged(this->updateCommand);
}

void CustomCommand::setInvokeCommand(const QString& command)
{
    this->invokeCommand = command;
    emit invokeCommandChanged(this->invokeCommand);
}

void CustomCommand::setClearCommand(const QString& command)
{
    this->clearCommand = command;
    emit clearCommandChanged(this->clearCommand);
}

void CustomCommand::setClearVideolayerCommand(const QString& command)
{
    this->clearVideolayerCommand = command;
    emit clearVideolayerCommandChanged(this->clearVideolayerCommand);
}

void CustomCommand::setClearChannelCommand(const QString& command)
{
    this->clearChannelCommand = command;
    emit clearChannelCommandChanged(this->clearChannelCommand);
}

void CustomCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"stopCommand") > 0) setStopCommand(QString::fromStdWString(pt.get<std::wstring>(L"stopCommand")));
    if (pt.count(L"playCommand") > 0) setPlayCommand(QString::fromStdWString(pt.get<std::wstring>(L"playCommand")));
    if (pt.count(L"loadCommand") > 0) setLoadCommand(QString::fromStdWString(pt.get<std::wstring>(L"loadCommand")));
    if (pt.count(L"pauseCommand") > 0) setPauseCommand(QString::fromStdWString(pt.get<std::wstring>(L"pauseCommand")));
    if (pt.count(L"nextCommand") > 0) setNextCommand(QString::fromStdWString(pt.get<std::wstring>(L"nextCommand")));
    if (pt.count(L"updateCommand") > 0) setUpdateCommand(QString::fromStdWString(pt.get<std::wstring>(L"updateCommand")));
    if (pt.count(L"invokeCommand") > 0) setInvokeCommand(QString::fromStdWString(pt.get<std::wstring>(L"invokeCommand")));
    if (pt.count(L"clearCommand") > 0) setClearCommand(QString::fromStdWString(pt.get<std::wstring>(L"clearCommand")));
    if (pt.count(L"clearVideolayerCommand") > 0) setClearVideolayerCommand(QString::fromStdWString(pt.get<std::wstring>(L"clearVideolayerCommand")));
    if (pt.count(L"clearChannelCommand") > 0) setClearChannelCommand(QString::fromStdWString(pt.get<std::wstring>(L"clearChannelCommand")));
}

void CustomCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("stopCommand", this->getStopCommand());
    writer->writeTextElement("playCommand", this->getPlayCommand());
    writer->writeTextElement("loadCommand", this->getLoadCommand());
    writer->writeTextElement("pauseCommand", this->getPauseCommand());
    writer->writeTextElement("nextCommand", this->getNextCommand());
    writer->writeTextElement("updateCommand", this->getUpdateCommand());
    writer->writeTextElement("invokeCommand", this->getInvokeCommand());
    writer->writeTextElement("clearCommand", this->getClearCommand());
    writer->writeTextElement("clearVideolayerCommand", this->getClearVideolayerCommand());
    writer->writeTextElement("clearChannelCommand", this->getClearChannelCommand());
}
