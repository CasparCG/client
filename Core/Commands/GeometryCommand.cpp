#include "GeometryCommand.h"

#include "Global.h"

GeometryCommand::GeometryCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), positionX(Mixer::DEFAULT_GEOMETRY_XPOS), positionY(Mixer::DEFAULT_GEOMETRY_YPOS),
      scaleX(Mixer::DEFAULT_GEOMETRY_XSCALE), scaleY(Mixer::DEFAULT_GEOMETRY_YSCALE), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int GeometryCommand::getDelay() const
{
    return this->delay;
}

int GeometryCommand::getChannel() const
{
    return this->channel;
}

int GeometryCommand::getVideolayer() const
{
    return this->videolayer;
}

void GeometryCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void GeometryCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void GeometryCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float GeometryCommand::getPositionX() const
{
    return this->positionX;
}

float GeometryCommand::getPositionY() const
{
    return this->positionY;
}

float GeometryCommand::getScaleX() const
{
    return this->scaleX;
}

float GeometryCommand::getScaleY() const
{
    return this->scaleY;
}

int GeometryCommand::getDuration() const
{
    return this->duration;
}

const QString& GeometryCommand::getTween() const
{
    return this->tween;
}

bool GeometryCommand::getDefer() const
{
    return this->defer;
}

void GeometryCommand::setPositionX(float positionX)
{
    this->positionX = positionX;
    emit positionXChanged(this->positionX);
}

void GeometryCommand::setPositionY(float positionY)
{
    this->positionY = positionY;
    emit positionYChanged(this->positionY);
}

void GeometryCommand::setScaleX(float scaleX)
{
    this->scaleX = scaleX;
    emit scaleXChanged(this->scaleX);
}

void GeometryCommand::setScaleY(float scaleY)
{
    this->scaleY = scaleY;
    emit scaleYChanged(this->scaleY);
}

void GeometryCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void GeometryCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void GeometryCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool GeometryCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void GeometryCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void GeometryCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setPositionX(pt.get<float>(L"positionx"));
    setPositionY(pt.get<float>(L"positiony"));
    setScaleX(pt.get<float>(L"scalex"));
    setScaleY(pt.get<float>(L"scaley"));
    setDuration(pt.get<int>(L"duration"));
    setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    setDefer(pt.get<bool>(L"defer"));
}

void GeometryCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("positionx", QString::number(this->getPositionX()));
    writer->writeTextElement("positiony", QString::number(this->getPositionY()));
    writer->writeTextElement("scalex", QString::number(this->getScaleX()));
    writer->writeTextElement("scaley", QString::number(this->getScaleY()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
