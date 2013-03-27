#include "VideoCommand.h"

#include "Global.h"

VideoCommand::VideoCommand(QObject* parent)
    : AbstractCommand(parent),
      videoName(Video::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Video::DEFAULT_LOOP),
      freezeOnLoad(Video::DEFAULT_FREEZE_ON_LOAD), triggerOnNext(Video::DEFAULT_TRIGGER_ON_NEXT), seek(Video::DEFAULT_SEEK),
      length(Video::DEFAULT_LENGTH), useAuto(Video::DEFAULT_USE_AUTO)
{
}

const QString& VideoCommand::getVideoName() const
{
    return this->videoName;
}

const QString& VideoCommand::getTransition() const
{
    return this->transition;
}

int VideoCommand::getDuration() const
{
    return this->duration;
}

const QString& VideoCommand::getDirection() const
{
    return this->direction;
}

const QString& VideoCommand::getTween() const
{
    return this->tween;
}

bool VideoCommand::getLoop() const
{
    return this->loop;
}

bool VideoCommand::getFreezeOnLoad() const
{
    return this->freezeOnLoad;
}

bool VideoCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

int VideoCommand::getSeek() const
{
    return this->seek;
}

int VideoCommand::getLength() const
{
    return this->length;
}

bool VideoCommand::getUseAuto() const
{
    return this->useAuto;
}

void VideoCommand::setVideoName(const QString& videoName)
{
    this->videoName = videoName;
    emit videoNameChanged(this->videoName);
}

void VideoCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void VideoCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void VideoCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void VideoCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void VideoCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);
}

void VideoCommand::setFreezeOnLoad(bool freezeOnLoad)
{
    this->freezeOnLoad = freezeOnLoad;
    emit freezeOnLoadChanged(this->freezeOnLoad);
}

void VideoCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void VideoCommand::setSeek(int seek)
{
    this->seek = seek;
    emit seekChanged(this->seek);
}

void VideoCommand::setLength(int length)
{
    this->length = length;
    emit lengthChanged(this->length);
}

void VideoCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void VideoCommand::readProperties(boost::property_tree::wptree& pt)
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

void VideoCommand::writeProperties(QXmlStreamWriter* writer)
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
