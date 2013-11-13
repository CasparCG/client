#include "GeometryCommand.h"

#include "Global.h"

GeometryCommand::GeometryCommand(QObject* parent)
    : AbstractCommand(parent),
      positionX(Mixer::DEFAULT_GEOMETRY_XPOS), positionY(Mixer::DEFAULT_GEOMETRY_YPOS), scaleX(Mixer::DEFAULT_GEOMETRY_XSCALE),
      scaleY(Mixer::DEFAULT_GEOMETRY_YSCALE), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), triggerOnNext(Geometry::DEFAULT_TRIGGER_ON_NEXT),
      defer(Mixer::DEFAULT_DEFER)
{
}

float GeometryCommand::getPositionX() const
{
    return this->positionX;
}

float GeometryCommand::getPositionY() const
{
    return this->positionY;
}

float GeometryCommand::getScaleX() const
{
    return this->scaleX;
}

float GeometryCommand::getScaleY() const
{
    return this->scaleY;
}

int GeometryCommand::getDuration() const
{
    return this->duration;
}

const QString& GeometryCommand::getTween() const
{
    return this->tween;
}

bool GeometryCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

bool GeometryCommand::getDefer() const
{
    return this->defer;
}

void GeometryCommand::setPositionX(float positionX)
{
    this->positionX = positionX;
    emit positionXChanged(this->positionX);
}

void GeometryCommand::setPositionY(float positionY)
{
    this->positionY = positionY;
    emit positionYChanged(this->positionY);
}

void GeometryCommand::setScaleX(float scaleX)
{
    this->scaleX = scaleX;
    emit scaleXChanged(this->scaleX);
}

void GeometryCommand::setScaleY(float scaleY)
{
    this->scaleY = scaleY;
    emit scaleYChanged(this->scaleY);
}

void GeometryCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void GeometryCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void GeometryCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void GeometryCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void GeometryCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setPositionX(pt.get(L"positionx", Mixer::DEFAULT_GEOMETRY_XPOS));
    setPositionY(pt.get(L"positiony", Mixer::DEFAULT_GEOMETRY_YPOS));
    setScaleX(pt.get(L"scalex", Mixer::DEFAULT_GEOMETRY_XSCALE));
    setScaleY(pt.get(L"scaley", Mixer::DEFAULT_GEOMETRY_YSCALE));
    setDuration(pt.get(L"duration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Geometry::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void GeometryCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("positionx", QString::number(this->getPositionX()));
    writer->writeTextElement("positiony", QString::number(this->getPositionY()));
    writer->writeTextElement("scalex", QString::number(this->getScaleX()));
    writer->writeTextElement("scaley", QString::number(this->getScaleY()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
