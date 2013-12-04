#include "AutoCommand.h"

#include "Global.h"

AutoCommand::AutoCommand(QObject* parent)
    : AbstractCommand(parent),
      step(TriCaster::DEFAULT_STEP), speed(TriCaster::DEFAULT_SPEED), transition(TriCaster::DEFAULT_TRANSITION)
{
}

void AutoCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);
}

void AutoCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);
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
