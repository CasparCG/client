#include "DeckLinkInputCommand.h"

#include "Global.h"

DeckLinkInputCommand::DeckLinkInputCommand(QObject* parent)
    : AbstractCommand(parent),
      device(DeckLinkInput::DEFAULT_DEVICE), format(DeckLinkInput::DEFAULT_FORMAT), transition(Mixer::DEFAULT_TRANSITION),
      duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION)
{
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
    AbstractCommand::readProperties(pt);

    if (pt.count(L"device") > 0) setDevice(pt.get<int>(L"device"));
    if (pt.count(L"format") > 0) setFormat(QString::fromStdWString(pt.get<std::wstring>(L"format")));
    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
}

void DeckLinkInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("device", QString::number(this->getDevice()));
    writer->writeTextElement("format", this->getFormat());
    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
}
