#include "AtemAutoCommand.h"

#include "Global.h"

AtemAutoCommand::AtemAutoCommand(QObject* parent)
    : AbstractCommand(parent),
      step(""), speed(Atem::DEFAULT_SPEED), transition(Atem::DEFAULT_TRANSITION), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemAutoCommand::getStep() const
{
    return this->step;
}

double AtemAutoCommand::getSpeed() const
{
    return this->speed;
}

const QString& AtemAutoCommand::getTransition() const
{
    return this->transition;
}

bool AtemAutoCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemAutoCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}

void AtemAutoCommand::setSpeed(double speed)
{
    this->speed = speed;
    emit speedChanged(this->speed);
}

void AtemAutoCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void AtemAutoCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemAutoCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", L"")));
    setSpeed(pt.get(L"speed", Atem::DEFAULT_SPEED));
    setTransition(QString::fromStdWString(pt.get(L"transition", Atem::DEFAULT_TRANSITION.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemAutoCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("step", this->getStep());
    writer->writeTextElement("speed", QString::number(this->getSpeed()));
    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
