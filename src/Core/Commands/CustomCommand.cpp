#include "CustomCommand.h"

#include "Global.h"

CustomCommand::CustomCommand(QObject* parent)
    : AbstractCommand(parent),
      stopCommand(""), playCommand(""), loadCommand(""), pauseCommand(""), nextCommand(""), updateCommand(""),
      invokeCommand(""), previewCommand(""), clearCommand(""), clearVideolayerCommand(""), clearChannelCommand(""),
      triggerOnNext(Custom::DEFAULT_TRIGGER_ON_NEXT)
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
    return this->nextCommand;
}

const QString& CustomCommand::getUpdateCommand() const
{
    return this->updateCommand;
}

const QString& CustomCommand::getInvokeCommand() const
{
    return this->invokeCommand;
}

const QString& CustomCommand::getPreviewCommand() const
{
    return this->previewCommand;
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

bool CustomCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
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

void CustomCommand::setPreviewCommand(const QString& command)
{
    this->previewCommand = command;
    emit previewCommandChanged(this->previewCommand);
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

void CustomCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void CustomCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStopCommand(QString::fromStdWString(pt.get(L"stopCommand", L"")));
    setPlayCommand(QString::fromStdWString(pt.get(L"playCommand", L"")));
    setLoadCommand(QString::fromStdWString(pt.get(L"loadCommand", L"")));
    setPauseCommand(QString::fromStdWString(pt.get(L"pauseCommand", L"")));
    setNextCommand(QString::fromStdWString(pt.get(L"nextCommand", L"")));
    setUpdateCommand(QString::fromStdWString(pt.get(L"updateCommand", L"")));
    setInvokeCommand(QString::fromStdWString(pt.get(L"invokeCommand", L"")));
    setPreviewCommand(QString::fromStdWString(pt.get(L"previewCommand", L"")));
    setClearCommand(QString::fromStdWString(pt.get(L"clearCommand", L"")));
    setClearVideolayerCommand(QString::fromStdWString(pt.get(L"clearVideolayerCommand", L"")));
    setClearChannelCommand(QString::fromStdWString(pt.get(L"clearChannelCommand", L"")));
    setTriggerOnNext(pt.get(L"triggeronnext", Custom::DEFAULT_TRIGGER_ON_NEXT));
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
    writer->writeTextElement("previewCommand", this->getPreviewCommand());
    writer->writeTextElement("clearCommand", this->getClearCommand());
    writer->writeTextElement("clearVideolayerCommand", this->getClearVideolayerCommand());
    writer->writeTextElement("clearChannelCommand", this->getClearChannelCommand());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
