#include "AutoCommand.h"

#include "Global.h"

AutoCommand::AutoCommand(QObject* parent)
    : AbstractCommand(parent),
      step(""), speed(TriCaster::DEFAULT_SPEED), transition(TriCaster::DEFAULT_TRANSITION)
{
}

const QString& AutoCommand::getStep() const
{
    return this->step;
}

const QString& AutoCommand::getSpeed() const
{
    return this->speed;
}

const QString& AutoCommand::getTransition() const
{
    return this->transition;
}

void AutoCommand::setStep(const QString& step)
{
    this->step = step;
    emit stepChanged(this->step);
}

void AutoCommand::setSpeed(const QString& speed)
{
    this->speed = speed;
    emit speedChanged(this->speed);
}

void AutoCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void AutoCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setStep(QString::fromStdWString(pt.get(L"step", L"")));
    setSpeed(QString::fromStdWString(pt.get(L"speed", TriCaster::DEFAULT_SPEED.toStdWString())));
    setTransition(QString::fromStdWString(pt.get(L"transition", TriCaster::DEFAULT_TRANSITION.toStdWString())));
}

void AutoCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("step", this->getStep());
    writer->writeTextElement("speed", this->getSpeed());
    writer->writeTextElement("transition", this->getTransition());
}
