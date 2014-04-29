#include "AudioCommand.h"

#include "Global.h"

#include "EventManager.h"

AudioCommand::AudioCommand(QObject* parent)
    : AbstractCommand(parent),
      audioName(Audio::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Audio::DEFAULT_LOOP),
      triggerOnNext(Audio::DEFAULT_TRIGGER_ON_NEXT), useAuto(Audio::DEFAULT_USE_AUTO)
{
    this->videolayer = Output::DEFAULT_AUDIO_VIDEOLAYER;
}

const QString& AudioCommand::getAudioName() const
{
    return this->audioName;
}

const QString& AudioCommand::getTransition() const
{
    return this->transition;
}

int AudioCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& AudioCommand::getDirection() const
{
    return this->direction;
}

const QString& AudioCommand::getTween() const
{
    return this->tween;
}

bool AudioCommand::getLoop() const
{
    return this->loop;
}

bool AudioCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool AudioCommand::getUseAuto() const
{
    return this->useAuto;
}

void AudioCommand::setAudioName(const QString& audioName)
{
    this->audioName = audioName;
    emit audioNameChanged(this->audioName);
}

void AudioCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void AudioCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void AudioCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void AudioCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void AudioCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);
}

void AudioCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AudioCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void AudioCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setLoop(pt.get(L"loop", Audio::DEFAULT_LOOP));
    setUseAuto(pt.get(L"useauto", Audio::DEFAULT_USE_AUTO));
    setTriggerOnNext(pt.get(L"triggeronnext", Audio::DEFAULT_TRIGGER_ON_NEXT));
}

void AudioCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", getTransition());
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("direction", getDirection());
    writer->writeTextElement("loop", (getLoop() == true) ? "true" : "false");
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
