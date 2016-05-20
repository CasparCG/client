#include "DeckLinkInputCommand.h"

#include <QtCore/QXmlStreamWriter>

DeckLinkInputCommand::DeckLinkInputCommand(QObject* parent)
    : AbstractCommand(parent)
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

int DeckLinkInputCommand::getTransitionDuration() const
{
    return this->transitionDuration;
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

void DeckLinkInputCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
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

    setDevice(pt.get(L"device", DeckLinkInput::DEFAULT_DEVICE));
    setFormat(QString::fromStdWString(pt.get(L"format", DeckLinkInput::DEFAULT_FORMAT.toStdWString())));
    setTransition(QString::fromStdWString(pt.get(L"transition", Mixer::DEFAULT_TRANSITION.toStdWString())));
    setTransitionDuration(pt.get(L"transitionDuration", pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION)));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDirection(QString::fromStdWString(pt.get(L"direction", Mixer::DEFAULT_DIRECTION.toStdWString())));
}

void DeckLinkInputCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("device", QString::number(getDevice()));
    writer->writeTextElement("format", getFormat());
    writer->writeTextElement("transition", getTransition());
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("direction", getDirection());
}
