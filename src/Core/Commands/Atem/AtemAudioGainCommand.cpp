#include "AtemAudioGainCommand.h"

#include "Global.h"

AtemAudioGainCommand::AtemAudioGainCommand(QObject* parent)
    : AbstractCommand(parent),
      source(Atem::DEFAULT_AUDIO_GAIN_SOURCE), gain(Atem::DEFAULT_AUDIO_GAIN), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemAudioGainCommand::getSource() const
{
    return this->source;
}

float AtemAudioGainCommand::getGain() const
{
    return this->gain;
}

bool AtemAudioGainCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemAudioGainCommand::setSource(const QString& source)
{
    this->source = source;
    emit sourceChanged(this->source);
}

void AtemAudioGainCommand::setGain(float gain)
{
    this->gain = gain;
    emit gainChanged(this->gain);
}

void AtemAudioGainCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemAudioGainCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setSource(QString::fromStdWString(pt.get(L"audiogainsource", Atem::DEFAULT_AUDIO_GAIN_SOURCE.toStdWString())));
    setGain(pt.get(L"gain", Atem::DEFAULT_AUDIO_GAIN));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemAudioGainCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("audiogainsource", getSource());
    writer->writeTextElement("gain", QString::number(getGain()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
