#include "InputCommand.h"

#include "Global.h"

InputCommand::InputCommand(QObject* parent)
    : AbstractCommand(parent),
      switcher(TriCaster::DEFAULT_SWITCHER), input(TriCaster::DEFAULT_INPUT), triggerOnNext(TriCaster::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& InputCommand::getSwitcher() const
{
    return this->switcher;
}

const QString& InputCommand::getInput() const
{
    return this->input;
}

bool InputCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void InputCommand::setSwitcher(const QString& switcher)
{
    this->switcher = switcher;
    emit switcherChanged(this->switcher);
}

void InputCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}

void InputCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void InputCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSwitcher(QString::fromStdWString(pt.get(L"switcher", TriCaster::DEFAULT_SWITCHER.toStdWString())));
    setInput(QString::fromStdWString(pt.get(L"input", TriCaster::DEFAULT_INPUT.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", TriCaster::DEFAULT_TRIGGER_ON_NEXT));
}

void InputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("switcher", this->getSwitcher());
    writer->writeTextElement("input", this->getInput());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
