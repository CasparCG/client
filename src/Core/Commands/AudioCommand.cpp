#include "AudioCommand.h"

#include "Global.h"

#include "EventManager.h"

AudioCommand::AudioCommand(QObject* parent)
    : AbstractCommand(parent),
      audioName(Audio::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Audio::DEFAULT_LOOP),
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

int AudioCommand::getDuration() const
{
    return this->duration;
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

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);

    EventManager::getInstance().fireRundownIsChangedEvent(true);
}

void AudioCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
    if (pt.count(L"loop") > 0) setLoop(pt.get<bool>(L"loop"));
    if (pt.count(L"useauto") > 0) setUseAuto(pt.get<bool>(L"useauto"));
    if (pt.count(L"triggeronnext") > 0) setTriggerOnNext(pt.get<bool>(L"triggeronnext"));
}

void AudioCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("loop", (getLoop() == true) ? "true" : "false");
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
