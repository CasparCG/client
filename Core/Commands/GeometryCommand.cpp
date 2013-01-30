#include "GeometryCommand.h"

#include "Global.h"

GeometryCommand::GeometryCommand(QObject* parent)
    : AbstractCommand(parent),
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
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"positionx") > 0) setPositionX(pt.get<float>(L"positionx"));
    if (pt.count(L"positiony") > 0) setPositionY(pt.get<float>(L"positiony"));
    if (pt.count(L"scalex") > 0) setScaleX(pt.get<float>(L"scalex"));
    if (pt.count(L"scaley") > 0) setScaleY(pt.get<float>(L"scaley"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
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
