#include "BrightnessCommand.h"

#include "Global.h"

BrightnessCommand::BrightnessCommand(QObject* parent)
    : AbstractCommand(parent),
      brightness(Mixer::DEFAULT_BRIGHTNESS), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float BrightnessCommand::getBrightness() const
{
    return this->brightness;
}

int BrightnessCommand::getDuration() const
{
    return this->duration;
}

const QString& BrightnessCommand::getTween() const
{
    return this->tween;
}

bool BrightnessCommand::getDefer() const
{
    return this->defer;
}

void BrightnessCommand::setBrightness(float brightness)
{
    this->brightness = brightness;
    emit brightnessChanged(this->brightness);
}

void BrightnessCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void BrightnessCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void BrightnessCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void BrightnessCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"brightness") > 0) setBrightness(pt.get<float>(L"brightness"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void BrightnessCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("brightness", QString::number(this->getBrightness()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
