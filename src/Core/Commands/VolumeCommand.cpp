#include "VolumeCommand.h"

#include <QtCore/QXmlStreamWriter>

VolumeCommand::VolumeCommand(QObject* parent)
    : AbstractCommand(parent)
{
    this->videolayer = Output::DEFAULT_AUDIO_VIDEOLAYER;
}

float VolumeCommand::getVolume() const
{
    return this->volume;
}

int VolumeCommand::getTransitionDuration() const
{
    return this->transitionDuration;
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

void VolumeCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
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

    setVolume(pt.get(L"volume", Mixer::DEFAULT_VOLUME));
    setTransitionDuration(pt.get(L"transitionDuration", pt.get(L"transtitionDuration", Mixer::DEFAULT_DURATION)));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void VolumeCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("volume", QString::number(getVolume()));
    writer.writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer.writeTextElement("tween", getTween());
    writer.writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
