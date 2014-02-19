#include "TakeCommand.h"

#include "Global.h"

TakeCommand::TakeCommand(QObject* parent)
    : AbstractCommand(parent),
      step(""), triggerOnNext(TriCaster::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& TakeCommand::getStep() const
{
    return this->step;
}

bool TakeCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void TakeCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}

void TakeCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void TakeCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", L"")));
    setTriggerOnNext(pt.get(L"triggeronnext", TriCaster::DEFAULT_TRIGGER_ON_NEXT));
}

void TakeCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("step", this->getStep());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
