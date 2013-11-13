#include "SaturationCommand.h"

#include "Global.h"

SaturationCommand::SaturationCommand(QObject* parent)
    : AbstractCommand(parent),
      saturation(Mixer::DEFAULT_SATURATION), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float SaturationCommand::getSaturation() const
{
    return this->saturation;
}

int SaturationCommand::getDuration() const
{
    return this->duration;
}

const QString& SaturationCommand::getTween() const
{
    return this->tween;
}

bool SaturationCommand::getDefer() const
{
    return this->defer;
}

void SaturationCommand::setSaturation(float saturation)
{
    this->saturation = saturation;
    emit saturationChanged(this->saturation);
}

void SaturationCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void SaturationCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void SaturationCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void SaturationCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSaturation(pt.get(L"saturation", Mixer::DEFAULT_SATURATION));
    setDuration(pt.get(L"duration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void SaturationCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("saturation", QString::number(this->getSaturation()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
