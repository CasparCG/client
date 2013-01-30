#include "ContrastCommand.h"

#include "Global.h"

ContrastCommand::ContrastCommand(QObject* parent)
    : AbstractCommand(parent),
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
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"contrast") > 0) setContrast(pt.get<float>(L"contrast"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
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
