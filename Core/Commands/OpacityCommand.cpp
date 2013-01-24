#include "OpacityCommand.h"

#include "Global.h"

OpacityCommand::OpacityCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), opacity(Mixer::DEFAULT_OPACITY), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int OpacityCommand::getDelay() const
{
    return this->delay;
}

int OpacityCommand::getChannel() const
{
    return this->channel;
}

int OpacityCommand::getVideolayer() const
{
    return this->videolayer;
}

void OpacityCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void OpacityCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void OpacityCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

float OpacityCommand::getOpacity() const
{
    return this->opacity;
}

int OpacityCommand::getDuration() const
{
    return this->duration;
}

const QString& OpacityCommand::getTween() const
{
    return this->tween;
}

bool OpacityCommand::getDefer() const
{
    return this->defer;
}

void OpacityCommand::setOpacity(float opacity)
{
    this->opacity = opacity;
    emit opacityChanged(this->opacity);
}

void OpacityCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void OpacityCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void OpacityCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

bool OpacityCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void OpacityCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void OpacityCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"opacity") > 0) setOpacity(pt.get<float>(L"opacity"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"tween") > 0) setDefer(pt.get<bool>(L"tween"));
}

void OpacityCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("opacity", QString::number(this->getOpacity()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
