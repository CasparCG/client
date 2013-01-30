#include "DeckLinkInputCommand.h"

#include "Global.h"

DeckLinkInputCommand::DeckLinkInputCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), device(DeckLinkInput::DEFAULT_DEVICE), format(DeckLinkInput::DEFAULT_FORMAT),
      transition(Mixer::DEFAULT_TRANSITION), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN),
      direction(Mixer::DEFAULT_DIRECTION)
{
}

int DeckLinkInputCommand::getDelay() const
{
    return this->delay;
}

int DeckLinkInputCommand::getChannel() const
{
    return this->channel;
}

int DeckLinkInputCommand::getVideolayer() const
{
    return this->videolayer;
}

bool DeckLinkInputCommand::getAllowGpi() const
{
    return this->allowGpi;
}

int DeckLinkInputCommand::getDevice() const
{
    return this->device;
}

const QString& DeckLinkInputCommand::getFormat() const
{
    return this->format;
}

const QString& DeckLinkInputCommand::getTransition() const
{
    return this->transition;
}

int DeckLinkInputCommand::getDuration() const
{
    return this->duration;
}

const QString& DeckLinkInputCommand::getDirection() const
{
    return this->direction;
}

const QString& DeckLinkInputCommand::getTween() const
{
    return this->tween;
}

void DeckLinkInputCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void DeckLinkInputCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void DeckLinkInputCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void DeckLinkInputCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void DeckLinkInputCommand::setDevice(int device)
{
    this->device = device;
    emit deviceChanged(this->device);
}

void DeckLinkInputCommand::setFormat(const QString& format)
{
    this->format = format;
    emit formatChanged(this->format);
}

void DeckLinkInputCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void DeckLinkInputCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void DeckLinkInputCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void DeckLinkInputCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void DeckLinkInputCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"videolayer") > 0) setVideolayer(pt.get<int>(L"videolayer"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"device") > 0) setDevice(pt.get<int>(L"device"));
    if (pt.count(L"format") > 0) setFormat(QString::fromStdWString(pt.get<std::wstring>(L"format")));
    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
}

void DeckLinkInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", QString::number(this->getVideolayer()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("device", QString::number(this->getDevice()));
    writer->writeTextElement("format", this->getFormat());
    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
}
