#include "HtmlCommand.h"

#include "Global.h"

HtmlCommand::HtmlCommand(QObject* parent)
    : AbstractCommand(parent),
      url(Html::DEFAULT_URL), transition(Mixer::DEFAULT_TRANSITION),
      transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION),
      freezeOnLoad(Html::DEFAULT_FREEZE_ON_LOAD), triggerOnNext(Html::DEFAULT_TRIGGER_ON_NEXT), useAuto(Html::DEFAULT_USE_AUTO)
{
}

const QString& HtmlCommand::getUrl() const
{
    return this->url;
}

const QString& HtmlCommand::getTransition() const
{
    return this->transition;
}

int HtmlCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& HtmlCommand::getDirection() const
{
    return this->direction;
}

const QString& HtmlCommand::getTween() const
{
    return this->tween;
}

bool HtmlCommand::getFreezeOnLoad() const
{
    return this->freezeOnLoad;
}

bool HtmlCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool HtmlCommand::getUseAuto() const
{
    return this->useAuto;
}

void HtmlCommand::setUrl(const QString& url)
{
    this->url = url;
    emit urlChanged(this->url);
}

void HtmlCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void HtmlCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void HtmlCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void HtmlCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void HtmlCommand::setFreezeOnLoad(bool freezeOnLoad)
{
    this->freezeOnLoad = freezeOnLoad;
    emit freezeOnLoadChanged(this->freezeOnLoad);
}

void HtmlCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void HtmlCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void HtmlCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setUrl(QString::fromStdWString(pt.get(L"url", Html::DEFAULT_URL.toStdWString())));
    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setFreezeOnLoad(pt.get(L"freezeonload", Html::DEFAULT_FREEZE_ON_LOAD));
    setTriggerOnNext(pt.get(L"triggeronnext", Html::DEFAULT_TRIGGER_ON_NEXT));
    setUseAuto(pt.get(L"useauto", Html::DEFAULT_USE_AUTO));
}

void HtmlCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("url", getUrl());
    writer->writeTextElement("transition", getTransition());
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("direction", getDirection());
    writer->writeTextElement("freezeonload", (getFreezeOnLoad() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
}
