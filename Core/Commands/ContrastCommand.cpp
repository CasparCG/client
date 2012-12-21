#include "ContrastCommand.h"

#include "Global.h"

ContrastCommand::ContrastCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), contrast(Mixer::DEFAULT_CONTRAST), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int ContrastCommand::getDelay() const
{
    return this->delay;
}

int ContrastCommand::getChannel() const
{
    return this->channel;
}

int ContrastCommand::getVideolayer() const
{
    return this->videolayer;
}

void ContrastCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void ContrastCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void ContrastCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float ContrastCommand::getContrast() const
{
    return this->contrast;
}

int ContrastCommand::getDuration() const
{
    return this->duration;
}

const QString& ContrastCommand::getTween() const
{
    return this->tween;
}

bool ContrastCommand::getDefer() const
{
    return this->defer;
}

void ContrastCommand::setContrast(float contrast)
{
    this->contrast = contrast;
    emit contrastChanged(this->contrast);
}

void ContrastCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void ContrastCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ContrastCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool ContrastCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void ContrastCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void ContrastCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setVideolayer(pt.get<int>(L"videolayer"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setContrast(pt.get<float>(L"contrast"));
    setDuration(pt.get<int>(L"duration"));
    setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    setDefer(pt.get<bool>(L"defer"));
}

void ContrastCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("contrast", QString::number(this->getContrast()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
