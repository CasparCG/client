#include "SaturationCommand.h"

#include "Global.h"

SaturationCommand::SaturationCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), saturation(Mixer::DEFAULT_SATURATION), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int SaturationCommand::getDelay() const
{
    return this->delay;
}

int SaturationCommand::getChannel() const
{
    return this->channel;
}

int SaturationCommand::getVideolayer() const
{
    return this->videolayer;
}

void SaturationCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void SaturationCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void SaturationCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float SaturationCommand::getSaturation() const
{
    return this->saturation;
}

int SaturationCommand::getDuration() const
{
    return this->duration;
}

const QString& SaturationCommand::getTween() const
{
    return this->tween;
}

bool SaturationCommand::getDefer() const
{
    return this->defer;
}

void SaturationCommand::setSaturation(float saturation)
{
    this->saturation = saturation;
    emit saturationChanged(this->saturation);
}

void SaturationCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void SaturationCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void SaturationCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool SaturationCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void SaturationCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void SaturationCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setSaturation(pt.get<float>(L"saturation"));
    setDuration(pt.get<int>(L"duration"));
    setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    setDefer(pt.get<bool>(L"defer"));
}

void SaturationCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("saturation", QString::number(this->getSaturation()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
