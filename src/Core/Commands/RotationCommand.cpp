#include "RotationCommand.h"

#include "Global.h"

RotationCommand::RotationCommand(QObject* parent)
    : AbstractCommand(parent),
      rotation(Mixer::DEFAULT_ROTATION), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN),
      triggerOnNext(Rotation::DEFAULT_TRIGGER_ON_NEXT), defer(Mixer::DEFAULT_DEFER)
{
}

float RotationCommand::getRotation() const
{
    return this->rotation;
}

int RotationCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

bool RotationCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

const QString& RotationCommand::getTween() const
{
    return this->tween;
}

bool RotationCommand::getDefer() const
{
    return this->defer;
}

void RotationCommand::setRotation(float rotation)
{
    this->rotation = rotation;
    emit rotationChanged(this->rotation);
}

void RotationCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void RotationCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void RotationCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void RotationCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void RotationCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setRotation(pt.get(L"rotation", Mixer::DEFAULT_ROTATION));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void RotationCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("rotation", QString::number(getRotation()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
