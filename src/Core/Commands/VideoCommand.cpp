#include "VideoCommand.h"

#include "Global.h"

VideoCommand::VideoCommand(QObject* parent)
    : AbstractCommand(parent),
      videoName(Video::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Video::DEFAULT_LOOP),
      freezeOnLoad(Video::DEFAULT_FREEZE_ON_LOAD), triggerOnNext(Video::DEFAULT_TRIGGER_ON_NEXT), seek(Video::DEFAULT_SEEK),
      length(Video::DEFAULT_LENGTH), autoPlay(Video::DEFAULT_AUTO_PLAY)
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

bool VideoCommand::getAutoPlay() const
{
    return this->autoPlay;
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

void VideoCommand::setAutoPlay(bool autoPlay)
{
    this->autoPlay = autoPlay;
    emit autoPlayChanged(this->autoPlay);
}

void VideoCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setDuration(pt.get(L"duration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setSeek(pt.get<int>(L"seek", Video::DEFAULT_SEEK));
    setLength(pt.get(L"length", Video::DEFAULT_LENGTH));
    setLoop(pt.get(L"loop", Video::DEFAULT_LOOP));
    setFreezeOnLoad(pt.get(L"freezeonload", Video::DEFAULT_FREEZE_ON_LOAD));
    setTriggerOnNext(pt.get(L"triggeronnext", Video::DEFAULT_TRIGGER_ON_NEXT));
    setAutoPlay(pt.get(L"autoplay", Video::DEFAULT_AUTO_PLAY));
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
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("autoplay", (getAutoPlay() == true) ? "true" : "false");
}
