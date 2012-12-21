#include "MediaCommand.h"

#include "Global.h"

MediaCommand::MediaCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), mediaName(Media::DEFAULT_MEDIA_NAME), transition(Mixer::DEFAULT_TRANSITION),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION),
      loop(Media::DEFAULT_LOOP), seek(Media::DEFAULT_SEEK), length(Media::DEFAULT_LENGTH)
{
}

int MediaCommand::getDelay() const
{
    return this->delay;
}

int MediaCommand::getChannel() const
{
    return this->channel;
}

int MediaCommand::getVideolayer() const
{
    return this->videolayer;
}

bool MediaCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void MediaCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void MediaCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void MediaCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void MediaCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

const QString& MediaCommand::getMediaName() const
{
    return this->mediaName;
}

const QString& MediaCommand::getTransition() const
{
    return this->transition;
}

int MediaCommand::getDuration() const
{
    return this->duration;
}

const QString& MediaCommand::getDirection() const
{
    return this->direction;
}

const QString& MediaCommand::getTween() const
{
    return this->tween;
}

bool MediaCommand::getLoop() const
{
    return this->loop;
}

int MediaCommand::getSeek() const
{
    return this->seek;
}

int MediaCommand::getLength() const
{
    return this->length;
}

void MediaCommand::setMediaName(const QString& mediaName)
{
    this->mediaName = mediaName;
    emit mediaNameChanged(this->mediaName);
}

void MediaCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void MediaCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void MediaCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void MediaCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void MediaCommand::setLoop(bool loop)
{
    this->loop = loop;
    emit loopChanged(this->loop);
}

void MediaCommand::setSeek(int seek)
{
    this->seek = seek;
    emit seekChanged(this->seek);
}

void MediaCommand::setLength(int length)
{
    this->length = length;
    emit lengthChanged(this->length);
}

void MediaCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    setDuration(pt.get<int>(L"duration"));
    setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
    setSeek(pt.get<int>(L"seek"));
    setLength(pt.get<int>(L"length"));
    setLoop(pt.get<bool>(L"loop"));
}

void MediaCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("seek", QString::number(this->getSeek()));
    writer->writeTextElement("length", QString::number(this->getLength()));
    writer->writeTextElement("loop", (getLoop() == true) ? "true" : "false");
}
