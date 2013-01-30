#include "VolumeCommand.h"

#include "Global.h"

VolumeCommand::VolumeCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), volume(Mixer::DEFAULT_VOLUME), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int VolumeCommand::getDelay() const
{
    return this->delay;
}

int VolumeCommand::getChannel() const
{
    return this->channel;
}

int VolumeCommand::getVideolayer() const
{
    return this->videolayer;
}

void VolumeCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void VolumeCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void VolumeCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float VolumeCommand::getVolume() const
{
    return this->volume;
}

int VolumeCommand::getDuration() const
{
    return this->duration;
}

const QString& VolumeCommand::getTween() const
{
    return this->tween;
}

bool VolumeCommand::getDefer() const
{
    return this->defer;
}

void VolumeCommand::setVolume(float volume)
{
    this->volume = volume;
    emit volumeChanged(this->volume);
}

void VolumeCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void VolumeCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void VolumeCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool VolumeCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void VolumeCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void VolumeCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"volume") > 0) setVolume(pt.get<float>(L"volume"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void VolumeCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("volume", QString::number(this->getVolume()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
