#include "MovieCommand.h"

#include "Global.h"

MovieCommand::MovieCommand(QObject* parent)
    : AbstractCommand(parent),
      videoName(Movie::DEFAULT_NAME), transition(Mixer::DEFAULT_TRANSITION),
      transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), loop(Movie::DEFAULT_LOOP),
      freezeOnLoad(Movie::DEFAULT_FREEZE_ON_LOAD), triggerOnNext(Movie::DEFAULT_TRIGGER_ON_NEXT), seek(Movie::DEFAULT_SEEK),
      length(Movie::DEFAULT_LENGTH), autoPlay(Movie::DEFAULT_AUTO_PLAY)
{
}

const QString& MovieCommand::getVideoName() const
{
    return this->videoName;
}

const QString& MovieCommand::getTransition() const
{
    return this->transition;
}

int MovieCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& MovieCommand::getDirection() const
{
    return this->direction;
}

const QString& MovieCommand::getTween() const
{
    return this->tween;
}

bool MovieCommand::getLoop() const
{
    return this->loop;
}

bool MovieCommand::getFreezeOnLoad() const
{
    return this->freezeOnLoad;
}

bool MovieCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

int MovieCommand::getSeek() const
{
    return this->seek;
}

int MovieCommand::getLength() const
{
    return this->length;
}

bool MovieCommand::getAutoPlay() const
{
    return this->autoPlay;
}

void MovieCommand::setVideoName(const QString& videoName)
{
    this->videoName = videoName;
    emit videoNameChanged(this->videoName);
}

void MovieCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void MovieCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void MovieCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void MovieCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void MovieCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);
}

void MovieCommand::setFreezeOnLoad(bool freezeOnLoad)
{
    this->freezeOnLoad = freezeOnLoad;
    emit freezeOnLoadChanged(this->freezeOnLoad);
}

void MovieCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void MovieCommand::setSeek(int seek)
{
    this->seek = seek;
    emit seekChanged(this->seek);
}

void MovieCommand::setLength(int length)
{
    this->length = length;
    emit lengthChanged(this->length);
}

void MovieCommand::setAutoPlay(bool autoPlay)
{
    this->autoPlay = autoPlay;
    emit autoPlayChanged(this->autoPlay);
}

void MovieCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
    setSeek(pt.get<int>(L"seek", Movie::DEFAULT_SEEK));
    setLength(pt.get(L"length", Movie::DEFAULT_LENGTH));
    setLoop(pt.get(L"loop", Movie::DEFAULT_LOOP));
    setFreezeOnLoad(pt.get(L"freezeonload", Movie::DEFAULT_FREEZE_ON_LOAD));
    setTriggerOnNext(pt.get(L"triggeronnext", Movie::DEFAULT_TRIGGER_ON_NEXT));
    setAutoPlay(pt.get(L"autoplay", Movie::DEFAULT_AUTO_PLAY));
}

void MovieCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", getTransition());
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("direction", getDirection());
    writer->writeTextElement("seek", QString::number(getSeek()));
    writer->writeTextElement("length", QString::number(getLength()));
    writer->writeTextElement("loop", (getLoop() == true) ? "true" : "false");
    writer->writeTextElement("freezeonload", (getFreezeOnLoad() == true) ? "true" : "false");
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("autoplay", (getAutoPlay() == true) ? "true" : "false");
}
