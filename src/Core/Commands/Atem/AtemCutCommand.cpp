#include "AtemCutCommand.h"

#include "Global.h"

AtemCutCommand::AtemCutCommand(QObject* parent)
    : AbstractCommand(parent),
      step("background"), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemCutCommand::getStep() const
{
    return this->step;
}

bool AtemCutCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemCutCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}

void AtemCutCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemCutCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", L"")));
    setTriggerOnNext(pt.get(L"triggeronnext", TriCaster::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemCutCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("step", this->getStep());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
