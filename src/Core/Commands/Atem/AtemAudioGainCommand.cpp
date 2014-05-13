#include "AtemAudioGainCommand.h"

#include "Global.h"

AtemAudioGainCommand::AtemAudioGainCommand(QObject* parent)
    : AbstractCommand(parent),
      input(Atem::DEFAULT_AUDIO_INPUT), gain(Atem::DEFAULT_AUDIO_GAIN), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemAudioGainCommand::getInput() const
{
    return this->input;
}

float AtemAudioGainCommand::getGain() const
{
    return this->gain;
}

bool AtemAudioGainCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemAudioGainCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
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

    setInput(QString::fromStdWString(pt.get(L"audiogaininput", Atem::DEFAULT_AUDIO_INPUT.toStdWString())));
    setGain(pt.get(L"gain", Atem::DEFAULT_AUDIO_GAIN));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemAudioGainCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("audiogaininput", getInput());
    writer->writeTextElement("gain", QString::number(getGain()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
