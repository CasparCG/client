#include "AtemAudioInputBalanceCommand.h"

#include "Global.h"

AtemAudioInputBalanceCommand::AtemAudioInputBalanceCommand(QObject* parent)
    : AbstractCommand(parent),
      input(Atem::DEFAULT_AUDIO_INPUT), balance(Atem::DEFAULT_AUDIO_INPUT_BALANCE), triggerOnNext(Atem::DEFAULT_TRIGGER_ON_NEXT)
{
}

const QString& AtemAudioInputBalanceCommand::getInput() const
{
    return this->input;
}

float AtemAudioInputBalanceCommand::getBalance() const
{
    return this->balance;
}

bool AtemAudioInputBalanceCommand::getTriggerOnNext() const
{
    return this->triggerOnNext;
}

void AtemAudioInputBalanceCommand::setInput(const QString& input)
{
    this->input = input;
    emit inputChanged(this->input);
}

void AtemAudioInputBalanceCommand::setBalance(float balance)
{
    this->balance = balance;
    emit balanceChanged(this->balance);
}

void AtemAudioInputBalanceCommand::setTriggerOnNext(bool triggerOnNext)
{
    this->triggerOnNext = triggerOnNext;
    emit triggerOnNextChanged(this->triggerOnNext);
}

void AtemAudioInputBalanceCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setInput(QString::fromStdWString(pt.get(L"audioinput", Atem::DEFAULT_AUDIO_INPUT.toStdWString())));
    setBalance(pt.get(L"balance", Atem::DEFAULT_AUDIO_INPUT_BALANCE));
    setTriggerOnNext(pt.get(L"triggeronnext", Atem::DEFAULT_TRIGGER_ON_NEXT));
}

void AtemAudioInputBalanceCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("audioinput", getInput());
    writer->writeTextElement("balance", QString::number(getBalance()));
    writer->writeTextElement("triggeronnext", (getTriggerOnNext() == true) ? "true" : "false");
}
