#include "ColorCommand.h"

#include "Global.h"

ColorCommand::ColorCommand(QObject* parent)
    : AbstractCommand(parent),
      color(Color::DEFAULT_COLOR_NAME), transition(Mixer::DEFAULT_TRANSITION), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), direction(Mixer::DEFAULT_DIRECTION), useAuto(Color::DEFAULT_USE_AUTO)
{
}

const QString& ColorCommand::getColor() const
{
    return this->color;
}

const QString& ColorCommand::getTransition() const
{
    return this->transition;
}

int ColorCommand::getDuration() const
{
    return this->duration;
}

const QString& ColorCommand::getDirection() const
{
    return this->direction;
}

const QString& ColorCommand::getTween() const
{
    return this->tween;
}

bool ColorCommand::getUseAuto() const
{
    return this->useAuto;
}

void ColorCommand::setColor(const QString& color)
{
    this->color = color;
    emit colorChanged(this->color);
}

void ColorCommand::setTransition(const QString& transition)
{
    this->transition = transition;
    emit transitionChanged(this->transition);
}

void ColorCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void ColorCommand::setDirection(const QString& direction)
{
    this->direction = direction;
    emit directionChanged(this->direction);
}

void ColorCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void ColorCommand::setUseAuto(bool useAuto)
{
    this->useAuto = useAuto;
    emit useAutoChanged(this->useAuto);
}

void ColorCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"transition") > 0) setTransition(QString::fromStdWString(pt.get<std::wstring>(L"transition")));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"direction") > 0) setDirection(QString::fromStdWString(pt.get<std::wstring>(L"direction")));
    if (pt.count(L"color") > 0) setColor(QString::fromStdWString(pt.get<std::wstring>(L"color")));
}

void ColorCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("transition", this->getTransition());
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("direction", this->getDirection());
    writer->writeTextElement("color", this->getColor());
}
