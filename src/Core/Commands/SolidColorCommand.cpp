#include "SolidColorCommand.h"

#include "Global.h"

SolidColorCommand::SolidColorCommand(QObject* parent)
    : AbstractCommand(parent),
      color(SolidColor::DEFAULT_COLOR), transition(Mixer::DEFAULT_TRANSITION), transtitionDuration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), useAuto(SolidColor::DEFAULT_USE_AUTO),
      triggerOnNext(SolidColor::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& SolidColorCommand::getColor() const
{
    return this->color;
}

const QString& SolidColorCommand::getTransition() const
{
    return this->transition;
}

int SolidColorCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

const QString& SolidColorCommand::getDirection() const
{
    return this->direction;
}

const QString& SolidColorCommand::getTween() const
{
    return this->tween;
}

bool SolidColorCommand::getUseAuto() const
{
    return this->useAuto;
}

bool SolidColorCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void SolidColorCommand::setColor(const QString& color)
{
    this->color = color;
    emit colorChanged(this->color);
}

void SolidColorCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void SolidColorCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void SolidColorCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void SolidColorCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void SolidColorCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void SolidColorCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void SolidColorCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setColor(QString::fromStdWString(pt.get(L"solidcolor", SolidColor::DEFAULT_COLOR.toStdWString())));
    setUseAuto(pt.get(L"useauto", SolidColor::DEFAULT_USE_AUTO));
    setTriggerOnNext(pt.get(L"triggeronnext", SolidColor::DEFAULT_TRIGGER_ON_NEXT));
}

void SolidColorCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", getTransition());
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("direction", getDirection());
    writer->writeTextElement("solidcolor", getColor());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
