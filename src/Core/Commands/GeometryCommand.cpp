#include "GeometryCommand.h"

#include "Global.h"

GeometryCommand::GeometryCommand(QObject* parent)
    : AbstractCommand(parent),
      positionX(Mixer::DEFAULT_GEOMETRY_XPOS), positionY(Mixer::DEFAULT_GEOMETRY_YPOS), scaleX(Mixer::DEFAULT_GEOMETRY_XSCALE),
      scaleY(Mixer::DEFAULT_GEOMETRY_YSCALE), transtitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), triggerOnNext(Geometry::DEFAULT_TRIGGER_ON_NEXT),
      defer(Mixer::DEFAULT_DEFER), useMipmap(Mixer::DEFAULT_MIPMAP)
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

int GeometryCommand::getTransitionDuration() const
{
    return this->transtitionDuration;
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

bool GeometryCommand::getUseMipmap() const
{
    return this->useMipmap;
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

void GeometryCommand::setTransitionDuration(int transtitionDuration)
{
    this->transtitionDuration = transtitionDuration;
    emit transtitionDurationChanged(this->transtitionDuration);
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

void GeometryCommand::setUseMipmap(bool useMipmap)
{
    this->useMipmap = useMipmap;
    emit useMipmapChanged(this->useMipmap);
}

void GeometryCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setPositionX(pt.get(L"positionx", Mixer::DEFAULT_GEOMETRY_XPOS));
    setPositionY(pt.get(L"positiony", Mixer::DEFAULT_GEOMETRY_YPOS));
    setScaleX(pt.get(L"scalex", Mixer::DEFAULT_GEOMETRY_XSCALE));
    setScaleY(pt.get(L"scaley", Mixer::DEFAULT_GEOMETRY_YSCALE));
    setTransitionDuration(pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Geometry::DEFAULT_TRIGGER_ON_NEXT));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
    setUseMipmap(pt.get(L"usemipmap", Mixer::DEFAULT_MIPMAP));
}

void GeometryCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("positionx", QString::number(getPositionX()));
    writer->writeTextElement("positiony", QString::number(getPositionY()));
    writer->writeTextElement("scalex", QString::number(getScaleX()));
    writer->writeTextElement("scaley", QString::number(getScaleY()));
    writer->writeTextElement("transtitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
    writer->writeTextElement("usemipmap", (getUseMipmap() == true) ? "true" : "false");
}
