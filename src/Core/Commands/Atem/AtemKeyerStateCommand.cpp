#include "AtemKeyerStateCommand.h"

#include "Global.h"

AtemKeyerStateCommand::AtemKeyerStateCommand(QObject* parent)
    : AbstractCommand(parent),
      keyer(Atem::DEFAULT_KEYER), state(Atem::DEFAULT_KEYER_STATE), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemKeyerStateCommand::getKeyer() const
{
    return this->keyer;
}

bool AtemKeyerStateCommand::getState() const
{
    return this->state;
}

bool AtemKeyerStateCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemKeyerStateCommand::setKeyer(const QString& keyer)
{
    this->keyer = keyer;
    emit keyerChanged(this->keyer);
}

void AtemKeyerStateCommand::setState(bool state)
{
    this->state = state;
    emit stateChanged(this->state);
}

void AtemKeyerStateCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemKeyerStateCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setKeyer(QString::fromStdWString(pt.get(L"keyer", Atem::DEFAULT_KEYER.toStdWString())));
    setState(pt.get(L"state", Atem::DEFAULT_KEYER_STATE));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemKeyerStateCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("keyer", this->getKeyer());
    writer->writeTextElement("state", (getState() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
