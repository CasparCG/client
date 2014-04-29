#include "OpacityCommand.h"

#include "Global.h"

OpacityCommand::OpacityCommand(QObject* parent)
    : AbstractCommand(parent),
      opacity(Mixer::DEFAULT_OPACITY), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), triggerOnNext(Opacity::DEFAULT_TRIGGER_ON_NEXT),
      defer(Mixer::DEFAULT_DEFER)
{
}

float OpacityCommand::getOpacity() const
{
    return this->opacity;
}

int OpacityCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
}

const QString& OpacityCommand::getTween() const
{
    return this->tween;
}

bool OpacityCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool OpacityCommand::getDefer() const
{
    return this->defer;
}

void OpacityCommand::setOpacity(float opacity)
{
    this->opacity = opacity;
    emit opacityChanged(this->opacity);
}

void OpacityCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
}

void OpacityCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void OpacityCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void OpacityCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void OpacityCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setOpacity(pt.get(L"opacity", Mixer::DEFAULT_OPACITY));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Opacity::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void OpacityCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("opacity", QString::number(getOpacity()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
