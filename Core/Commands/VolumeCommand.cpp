#include "VolumeCommand.h"

#include "Global.h"

VolumeCommand::VolumeCommand(QObject* parent)
    : AbstractCommand(parent),
      volume(Mixer::DEFAULT_VOLUME), duration(Mixer::DEFAULT_DURATION),
      tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

float VolumeCommand::getVolume() const
{
    return this->volume;
}

int VolumeCommand::getDuration() const
{
    return this->duration;
}

const QString& VolumeCommand::getTween() const
{
    return this->tween;
}

bool VolumeCommand::getDefer() const
{
    return this->defer;
}

void VolumeCommand::setVolume(float volume)
{
    this->volume = volume;
    emit volumeChanged(this->volume);
}

void VolumeCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void VolumeCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void VolumeCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void VolumeCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"volume") > 0) setVolume(pt.get<float>(L"volume"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void VolumeCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("volume", QString::number(this->getVolume()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
