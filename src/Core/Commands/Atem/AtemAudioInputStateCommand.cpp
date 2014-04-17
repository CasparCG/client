#include "AtemAudioInputStateCommand.h"

#include "Global.h"

AtemAudioInputStateCommand::AtemAudioInputStateCommand(QObject* parent)
    : AbstractCommand(parent),
      input(Atem::DEFAULT_AUDIO_INPUT), state(Atem::DEFAULT_AUDIO_INPUT_STATE), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemAudioInputStateCommand::getInput() const
{
    return this->input;
}

const QString& AtemAudioInputStateCommand::getState() const
{
    return this->state;
}

bool AtemAudioInputStateCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemAudioInputStateCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}

void AtemAudioInputStateCommand::setState(const QString& state)
{
    this->state = state;
    emit stateChanged(this->state);
}

void AtemAudioInputStateCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemAudioInputStateCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setInput(QString::fromStdWString(pt.get(L"audioinput", Atem::DEFAULT_AUDIO_INPUT.toStdWString())));
    setState(QString::fromStdWString(pt.get(L"state", Atem::DEFAULT_AUDIO_INPUT_STATE.toStdWString())));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemAudioInputStateCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("audioinput", getInput());
    writer->writeTextElement("state", getState());
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
