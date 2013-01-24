#include "BrightnessCommand.h"

#include "Global.h"

BrightnessCommand::BrightnessCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), brightness(Mixer::DEFAULT_BRIGHTNESS), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int BrightnessCommand::getDelay() const
{
    return this->delay;
}

int BrightnessCommand::getChannel() const
{
    return this->channel;
}

int BrightnessCommand::getVideolayer() const
{
    return this->videolayer;
}

void BrightnessCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void BrightnessCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void BrightnessCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float BrightnessCommand::getBrightness() const
{
    return this->brightness;
}

int BrightnessCommand::getDuration() const
{
    return this->duration;
}

const QString& BrightnessCommand::getTween() const
{
    return this->tween;
}

bool BrightnessCommand::getDefer() const
{
    return this->defer;
}

void BrightnessCommand::setBrightness(float brightness)
{
    this->brightness = brightness;
    emit brightnessChanged(this->brightness);
}

void BrightnessCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void BrightnessCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void BrightnessCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool BrightnessCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void BrightnessCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void BrightnessCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"brightness") > 0) setBrightness(pt.get<float>(L"brightness"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void BrightnessCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("brightness", QString::number(this->getBrightness()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
