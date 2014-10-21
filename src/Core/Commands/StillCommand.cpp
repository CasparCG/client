#include "StillCommand.h"

#include "Global.h"

StillCommand::StillCommand(QObject* parent)
    : AbstractCommand(parent),
      imageName(Still::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION),
      triggerOnNext(Still::DEFAULT_TRIGGER_ON_NEXT), useAuto(Still::DEFAULT_USE_AUTO)
{
}

const QString& StillCommand::getImageName() const
{
    return this->imageName;
}

const QString& StillCommand::getTransition() const
{
    return this->transition;
}

int StillCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& StillCommand::getDirection() const
{
    return this->direction;
}

const QString& StillCommand::getTween() const
{
    return this->tween;
}

bool StillCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool StillCommand::getUseAuto() const
{
    return this->useAuto;
}

void StillCommand::setImageName(const QString& imageName)
{
    this->imageName = imageName;
    emit imageNameChanged(this->imageName);
}

void StillCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void StillCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void StillCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void StillCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void StillCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void StillCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void StillCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setUseAuto(pt.get(L"useauto", Still::DEFAULT_USE_AUTO));
    setTriggerOnNext(pt.get(L"triggeronnext", Still::DEFAULT_TRIGGER_ON_NEXT));
}

void StillCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("transitionDuration", QString::number(this->getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
