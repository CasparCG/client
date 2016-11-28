#include "AnchorCommand.h"

#include <QtCore/QXmlStreamWriter>

AnchorCommand::AnchorCommand(QObject* parent)
    : AbstractCommand(parent)
{
}

float AnchorCommand::getPositionX() const
{
    return this->positionX;
}

float AnchorCommand::getPositionY() const
{
    return this->positionY;
}

int AnchorCommand::getTransitionDuration() const
{
    return this->transitionDuration;
}

const QString& AnchorCommand::getTween() const
{
    return this->tween;
}

bool AnchorCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool AnchorCommand::getDefer() const
{
    return this->defer;
}

void AnchorCommand::setPositionX(float positionX)
{
    this->positionX = positionX;
    emit positionXChanged(this->positionX);
}

void AnchorCommand::setPositionY(float positionY)
{
    this->positionY = positionY;
    emit positionYChanged(this->positionY);
}

void AnchorCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
}

void AnchorCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void AnchorCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AnchorCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void AnchorCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setPositionX(pt.get(L"positionx", Mixer::DEFAULT_FILL_XPOS));
    setPositionY(pt.get(L"positiony", Mixer::DEFAULT_FILL_YPOS));
    setTransitionDuration(pt.get(L"transitionDuration", pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION)));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Fill::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void AnchorCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("positionx", QString::number(getPositionX()));
    writer.writeTextElement("positiony", QString::number(getPositionY()));
    writer.writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer.writeTextElement("tween", getTween());
    writer.writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer.writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
