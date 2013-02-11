#include "GeometryCommand.h"

#include "Global.h"

GeometryCommand::GeometryCommand(QObject* parent)
    : AbstractCommand(parent),
      positionX(Mixer::DEFAULT_GEOMETRY_XPOS), positionY(Mixer::DEFAULT_GEOMETRY_YPOS), scaleX(Mixer::DEFAULT_GEOMETRY_XSCALE),
      scaleY(Mixer::DEFAULT_GEOMETRY_YSCALE), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
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

void GeometryCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void GeometryCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"positionx") > 0) setPositionX(pt.get<float>(L"positionx"));
    if (pt.count(L"positiony") > 0) setPositionY(pt.get<float>(L"positiony"));
    if (pt.count(L"scalex") > 0) setScaleX(pt.get<float>(L"scalex"));
    if (pt.count(L"scaley") > 0) setScaleY(pt.get<float>(L"scaley"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
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
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
