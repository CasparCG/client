#include "ColorProducerCommand.h"

#include "Global.h"

ColorProducerCommand::ColorProducerCommand(QObject* parent)
    : AbstractCommand(parent),
      mediaName(Media::DEFAULT_MEDIA_NAME), transition(Mixer::DEFAULT_TRANSITION), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Media::DEFAULT_LOOP),
      freezeOnLoad(Media::DEFAULT_FREEZE_ON_LOAD), triggerOnNext(Media::DEFAULT_TRIGGER_ON_NEXT), seek(Media::DEFAULT_SEEK),
      length(Media::DEFAULT_LENGTH), useAuto(Media::DEFAULT_USE_AUTO)
{
}

const QString& ColorProducerCommand::getMediaName() const
{
    return this->mediaName;
}

const QString& ColorProducerCommand::getTransition() const
{
    return this->transition;
}

int ColorProducerCommand::getDuration() const
{
    return this->duration;
}

const QString& ColorProducerCommand::getDirection() const
{
    return this->direction;
}

const QString& ColorProducerCommand::getTween() const
{
    return this->tween;
}

bool ColorProducerCommand::getLoop() const
{
    return this->loop;
}

bool ColorProducerCommand::getFreezeOnLoad() const
{
    return this->freezeOnLoad;
}

bool ColorProducerCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

int ColorProducerCommand::getSeek() const
{
    return this->seek;
}

int ColorProducerCommand::getLength() const
{
    return this->length;
}

bool ColorProducerCommand::getUseAuto() const
{
    return this->useAuto;
}

void ColorProducerCommand::setMediaName(const QString& mediaName)
{
    this->mediaName = mediaName;
    emit mediaNameChanged(this->mediaName);
}

void ColorProducerCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void ColorProducerCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void ColorProducerCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void ColorProducerCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ColorProducerCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);
}

void ColorProducerCommand::setFreezeOnLoad(bool freezeOnLoad)
{
    this->freezeOnLoad = freezeOnLoad;
    emit freezeOnLoadChanged(this->freezeOnLoad);
}

void ColorProducerCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void ColorProducerCommand::setSeek(int seek)
{
    this->seek = seek;
    emit seekChanged(this->seek);
}

void ColorProducerCommand::setLength(int length)
{
    this->length = length;
    emit lengthChanged(this->length);
}

void ColorProducerCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void ColorProducerCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
    if (pt.count(L"seek") > 0) setSeek(pt.get<int>(L"seek"));
    if (pt.count(L"length") > 0) setLength(pt.get<int>(L"length"));
    if (pt.count(L"loop") > 0) setLoop(pt.get<bool>(L"loop"));
    if (pt.count(L"freezeonload") > 0) setFreezeOnLoad(pt.get<bool>(L"freezeonload"));
    if (pt.count(L"useauto") > 0) setUseAuto(pt.get<bool>(L"useauto"));
    if (pt.count(L"triggeronnext") > 0) setTriggerOnNext(pt.get<bool>(L"triggeronnext"));
}

void ColorProducerCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("seek", QString::number(this->getSeek()));
    writer->writeTextElement("length", QString::number(this->getLength()));
    writer->writeTextElement("loop", (getLoop() == true) ? "true" : "false");
    writer->writeTextElement("freezeonload", (getFreezeOnLoad() == true) ? "true" : "false");
    writer->writeTextElement("useauto", (getUseAuto() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
